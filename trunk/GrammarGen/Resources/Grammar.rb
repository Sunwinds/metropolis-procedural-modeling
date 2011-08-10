require 'pp'

class Grammar

  def initialize(fileName,turtleSymbols,mathSymbols,probDists)
    @fullFilePath = fileName
    @turtleSymbols = turtleSymbols
    @mathSymbols = mathSymbols
    @probDists = probDists
    @derivationLength
    @terminals = []
    @nonTerminals = []
    @distTerminals = []
    @symbolHash = {}
    @nonTerminalHash = {}
    @convertedProductions = {}
    @convertedDistTerminals = {}
    @numProductionParams = {}
    @includesList = []
    @arrays = {}
    @tableOffset = 100
  end

  attr_accessor :terminals, :nonTerminals,:symbolHash

  def setDerivationLength(length)
    @derivationLength = length
  end

  def addNonTerminal(nonTerminal)
    if !nonTerminal then return end
    safeToAdd = true
    toAdd = [ nonTerminal[0], nonTerminal[1].size ]
    @nonTerminals.each do |nt|
      if nt[0] == toAdd[0] and nt[1] == toAdd[1] then safeToAdd = false
      elsif nt[0] == toAdd[0] and nt[1] != toAdd[1]
        raise Exception.new("Adding non-terminals: #{nt[0]} already exists as non-terminal with #{nt[1]} parameters (not #{toAdd[1]})")
      end
    end
    if not safeToAdd then return
    else @nonTerminals << toAdd end
  end

  def processSymbol(s)
    symbol = [ s[0], s[1].size ]
    containsDistribution = false
    s[1].each do |expression|
      containsDistribution = containsDistribution || expression.inject(false){ |cum,curr| cum = cum || curr.class.to_s == "Distribution" }
    end
    if containsDistribution then if not @distTerminals.member?(s) then @distTerminals << s end end
    isTerminal = true
    @nonTerminals.each do |nt|
      if nt[0] == symbol[0] and nt[1] == symbol[1] then isTerminal = false
      elsif nt[0] == symbol[0] and nt[1] != symbol[1]
        raise Exception.new("Adding production: #{nt[0]} already exists as non-terminal with #{nt[1]} parameters (not #{symbol[1]})")
      end
    end
    if not isTerminal
      if containsDistribution then raise Exception.new("Adding production: #{s[0]} contains a distribution, but is not a terminal symbol")
      else return end
    end
    isTurtle = false
    @turtleSymbols.each do |ts|
      if ts[0] == symbol[0] and ts[1] == symbol[1] then isTurtle = true
      elsif ts[0] == symbol[0] and ts[1] != symbol[1]
        raise Exception.new("Adding production: #{ts[0]} is a turtle symbol with #{ts[1]} parameters (not #{symbol[1]})")
      end
    end
    if isTurtle then return end
    terminalExists = false
    @terminals.each do |t|
      if t[0] == symbol[0] and t[1] == symbol[1] then terminalExists = true
      elsif t[0] == symbol[0] and t[1] != symbol[1]
        raise Exception.new("Adding production: #{t[0]} already exists as terminal with #{t[1]} parameters (not #{symbol[1]})")
      end
    end
    if terminalExists then return end
    @terminals << symbol
  end

  def addProductions(productions)
    @convertedProductions[0] = productions
    productions.each { |p| successorList = p.successor; successorList.each { |s| processSymbol(s) } }
  end

  def addAxiom(axiom)
    @axiom = axiom
    axiom.each { |s| processSymbol(s) }
  end

  def addArrays(arrays)
    arrays.each do |name, list|
      @arrays[name] = list
    end
  end

  def createSymbolHash
    index = -1
    @turtleSymbols.each { |s| @symbolHash[s[0]] = index += 1 }
    index = -1
    @nonTerminals.each { |s| id = 1*@tableOffset + index += 1; @symbolHash[s[0]] = id; @nonTerminalHash[id] = s[0] }
    index = -1
    @terminals.each { |s| @symbolHash[s[0]] = 2*@tableOffset + index += 1 }
    index = -1
    @distTerminals.each { |s| @symbolHash[s] = 3*@tableOffset + index += 1 }
    index = -1
  end

  def numSymbols
    return @turtleSymbols.size + @terminals.size + @nonTerminals.size + @distTerminals.size
  end

  def convertSymbol(paramHash,s)
    if paramHash[s] then "params[#{paramHash[s]}]"
    elsif @mathSymbols.member?(s) then s
    elsif s.class.to_s == "ParamArray"
      if not @arrays[s.name] then raise Exception.new("Array @#{s.name} not declared") end
      s.indexExpression.map! do |e|
        begin
          convertSymbol(paramHash,e)
        rescue Exception => err
          raise Exception.new(err.to_s + " in array #{s.name}")
        end
      end
      "Arrays::#{s.name}[(int)(#{s.indexExpression.join})]"
    else
      begin
        val = eval(s)
      rescue Exception => err
        raise Exception.new("Unbound or unrecognized symbol #{s}")
      end
      if val.class.to_s != "Fixnum" and val.class.to_s != "Float" then raise Exception.new("Unbound or unrecognized symbol #{s}") end
      val.to_f
    end
  end

  def convertDistTerminalSymbol(s,paramHash,distList,distSymbol)
    neccParamsHash = {}
    s[1].each do |c|
      c.each do |t|
        if paramHash[t] then neccParamsHash[t] = neccParamsHash.size
        else
          if t.class.to_s == "Distribution"
          t.boundsArray[0].each { |nt| if paramHash[nt] then neccParamsHash[nt] = neccParamsHash.size end }
          t.boundsArray[1].each { |nt| if paramHash[nt] then neccParamsHash[nt] = neccParamsHash.size end }
          end
        end
      end
    end

    neccParams = []
    orderedParams = paramHash.sort { |a,b| a[1] <=> b[1] }
    index = 0
    orderedParams.each do |p,i|
      if neccParamsHash[p]
        neccParams << "params[#{i}]"
        neccParamsHash[p] = index + distList.size
        index += 1
      end
    end

    diffParams = []
    numStoredDiffusionParams = 0
    s[1].map! do |c|
      c.map! do |t|
        if t.class.to_s == "Distribution"
          numUnifs = 0
          @probDists.each { |name, args, unifs| if t.name == name and t.boundsArray.size == args then numUnifs = unifs end }
          actualParam = "Distributions::" + t.name + '('
          t.boundsArray.each do |expr|
            expr.map! { |nt| convertSymbol(neccParamsHash,nt) }
            actualParam += expr.join + ','
          end
          [*1..numUnifs].each do |i|
            actualParam += "params[#{numStoredDiffusionParams+i-1}],"
          end
          numStoredDiffusionParams += numUnifs
          diffParams << [ actualParam[0..-2] + ')' ]
          actualParam[0..-2] + ')'
        else convertSymbol(neccParamsHash,t) end
      end
      c.join
    end

    toAdd = [distSymbol, s[1], diffParams, numStoredDiffusionParams, neccParams.size]

    if @convertedDistTerminals[s[0]] then @convertedDistTerminals[s[0]] = @convertedDistTerminals[s[0]] << toAdd
    else @convertedDistTerminals[s[0]] = [ toAdd ] end

    [ distSymbol, distList, neccParams ]
  end

  def convertGrammarSymbol(symbol,paramHash,listName)
    s=deepCopy(symbol)
    sCopy=deepCopy(symbol)

    distSymbol = @symbolHash[symbol]

    distList = []

    s[1].map! do |c|
      c.map! do |t|
        if t.class.to_s == "Distribution"
          isValidDist = false
          numUnifs = 0
          @probDists.each { |name, args, unifs| if t.name == name and t.boundsArray.size == args then isValidDist = true; numUnifs = unifs end }
          if not isValidDist then raise Exception.new("#{t.name} is not a supported distribution") end
          t.boundsArray.map! { |expr| expr.map! { |nt| convertSymbol(paramHash,nt) } }
          [*1..numUnifs].each { distList << 'Math::uniformZeroToOne()' }
          listName[0] + "[" + distList.size.to_s + "]" + listName[1]
        else convertSymbol(paramHash,t) end
      end
      c.join
    end

    if distSymbol then convertDistTerminalSymbol(sCopy,paramHash,distList,distSymbol)
    else [ @symbolHash[s[0]], distList, s[1] ] end
  end

  def convertAxiom
    @axiom.map! { |s| convertGrammarSymbol(s,{},["axiom",""]) }
  end

  def convertProductions
    productions = @convertedProductions[0]
    @convertedProductions.clear
    productions.each do |production|

      pred=production.predecessor
      cond=production.conditional
      prob=production.probability
      succ=production.successor

      paramHash = {}

      pred[1].each_with_index do |p,i|
        if @mathSymbols.member?(p) then raise Exception.new("Cannot use #{p} as a parameter name: symbol is reserved") end
        paramHash[p] = i
      end

      cond.map! do |t|
        begin
          convertSymbol(paramHash,t)
        rescue Exception => err
          raise Exception.new("Unbound variable #{t} in probability from #{pred[0]} with params #{pred[1]}\n " + err.to_s)
        end
      end

      prob.map! do |t|
        begin
          convertSymbol(paramHash,t)
        rescue Exception => err
          raise Exception.new("Unbound variable #{t} in probability from #{pred[0]} with params #{pred[1]}\n " + err.to_s)
        end
      end

      succ.map! { |s| convertGrammarSymbol(s, paramHash, ["output",".mParam" ]) }

      @numProductionParams[pred[0]] = pred[1].size
      if @convertedProductions[pred[0]] then @convertedProductions[pred[0]] = @convertedProductions[pred[0]] << [ prob, succ, cond ]
      else @convertedProductions[pred[0]] = [ [ prob, succ, cond ] ] end
    end
  end

  def writeSymbols(targetDirectory,fileName,savedRenderMethods,savedIncludes,savedInitGraphics,savedSaveToMesh,savedVoxel)

    tabSpace = "    "

    distTerminalStubFileContents = File.open("Resources/DistTerminalStub").readlines.join
    stubFileContents = File.open("Resources/SymbolStub").readlines.join
    arraysFileContents = File.open("Resources/ArraysStub").readlines.join

    arraysFile = File.open(targetDirectory+"Arrays.hpp","w")
    arraysString = ""
    @arrays.each do |name,items|
      arraysString = arraysString + "float #{name}[] = {#{items.join(",")}};\n"
    end
    arraysFileContents.gsub!("%ARRAYS%",arraysString)
    arraysFile.write(arraysFileContents)
    arraysFile.close

    @convertedProductions.each do |p,rest|

      @includesList << "#{p}Symbol.hpp"

      symbolFile = File.open(targetDirectory+p+"Symbol.hpp","w")
      symbolFileContents = stubFileContents.dup

      symbolFileContents.gsub!("%TERMINAL%","false")
      symbolFileContents.gsub!("%SYMBOLNAME%",p)
      symbolFileContents.gsub!("%HEADERGUARD%",p.upcase)
      symbolFileContents.gsub!("%RENDER%",savedRenderMethods[p] ? savedRenderMethods[p] : "")
      symbolFileContents.gsub!("%INCLUDES%",savedIncludes[p] ? savedIncludes[p] : "")
      symbolFileContents.gsub!("%INITGRAPHICS%",savedInitGraphics[p] ? savedInitGraphics[p] : "")
      symbolFileContents.gsub!("%SAVETOMESH%",savedSaveToMesh[p] ? savedSaveToMesh[p] : "")
      symbolFileContents.gsub!("%VOXEL%",savedVoxel[p] ? savedVoxel[p] : "")

      productionString = ""
      isValidString = ""

      numSymbolEntries = [@numProductionParams[p] + 1]
      numSymbols = []
      if rest.size == 1
        index = 0

        cond=rest[0][2].join

        if cond != "true"
          productionString += tabSpace*2 + "if(input[0].isInvalid()){ output[0].mID=#{@symbolHash[p]}; memcpy(output+1,input,sizeof(SymbolEntry)*#{@numProductionParams[p]+1}); logProbability=0; return #{@numProductionParams[p]+2}; }\n\n"
          isValidString="return SymbolEntry::DETERMINISTIC | (#{cond} ?  SymbolEntry::CLEAR : SymbolEntry::INVALID);"
        else
          isValidString="return SymbolEntry::DETERMINISTIC;"
        end

        productionString += tabSpace*2 + "const float* params=reinterpret_cast<const float*>(input+1);\n\n"
        rest[0][1].each do |s|
          productionString += tabSpace*2 + "output[#{index}].mID=#{s[0]};\n"
          if @nonTerminalHash[s[0]] then validIndex=index=index+1 end
          [*1..s[1].size].each { |i| productionString += tabSpace*2 + "output[#{index+i}].mParam=#{s[1][i-1]};\n" }
          index += s[1].size
          [*1..s[2].size].each { |i| productionString += tabSpace*2 + "output[#{index+i}].mParam=#{s[2][i-1]};\n" }
          index += s[2].size+ 1
          if @nonTerminalHash[s[0]] then productionString += tabSpace*2 + "output[#{validIndex}].mFlags=mSymbolTable[#{s[0]}]->setFlags(reinterpret_cast<float*>(output+#{validIndex+1}));\n" end
        end

        numSymbols << rest[0][1].size
        numSymbolEntries << index
        productionString = productionString[8..-1]
        productionString += tabSpace*2 + "logProbability=0;\n"
        productionString += tabSpace*2 + "return #{index};"
      else
        productionString = tabSpace*2 + "switch(selector)\n" + tabSpace*2 + "{\n"
        condSumLine = "float prob[#{rest.size}]={\n"
        sumLine = "float prob[#{rest.size}]={\n"
        condLine = "bool cond[#{rest.size}]={\n"
        selectorLine = "uint selector=discreteSample(#{rest.size},"
        trivialConditionals = true

        isValidString = "uint conditionals=0;\n"
        rest.each_with_index do |production,productionIndex|
          cond = production[2].join
          trivialConditionals &&= cond == "true"
          isValidString += tabSpace*2 + "if(#{production[2].join}) conditionals++;\n"
          condLine += tabSpace*3 + "#{production[2].join},\n";
          condSumLine += tabSpace*3 + "cond[#{productionIndex}]?Math::clampNonNegative(#{production[0].join}):0,\n";
          sumLine += tabSpace*3 + "Math::clampNonNegative(#{production[0].join}),\n";
          selectorLine += "prob[#{productionIndex}],"
          productionString += tabSpace*3 + "case #{productionIndex}:\n"

          index = 0
          production[1].each do |s|
            productionString += tabSpace*4 + "output[#{index}].mID=#{s[0]};\n"
            if @nonTerminalHash[s[0]] then validIndex=index=index+1 end
            [*1..s[1].size].each { |i| productionString += tabSpace*4 + "output[#{index+i}].mParam=#{s[1][i-1]};\n" }
            index += s[1].size
            [*1..s[2].size].each { |i| productionString += tabSpace*4 + "output[#{index+i}].mParam=#{s[2][i-1]};\n" }
            index += s[2].size+ 1
            if @nonTerminalHash[s[0]] then productionString += tabSpace*4 + "output[#{validIndex}].mFlags=mSymbolTable[#{s[0]}]->setFlags(reinterpret_cast<float*>(output+#{validIndex+1}));\n" end
          end
          numSymbols << production[1].size
          numSymbolEntries << index
          productionString += tabSpace*4 + "logProbability=log(prob[#{productionIndex}]);\n"
          productionString += tabSpace*4 + "return #{index};\n"
          productionString += tabSpace*3 + "break;\n"
        end

        if trivialConditionals
          if rest.size==1 then isValidString = "return SymbolEntry::DETERMINISTIC;" else isValidString = "return SymbolEntry::CLEAR;" end
        else
          isValidString += "\n" + tabSpace*2 + "switch(conditionals)\n" + tabSpace*2 +"{\n" + tabSpace*3 + "case 0: return SymbolEntry::INVALID; break;\n"
          isValidString += tabSpace*3 + "case 1: return SymbolEntry::DETERMINISTIC; break;\n" + tabSpace*3 + "default: return SymbolEntry::CLEAR; break;\n"
          isValidString += tabSpace*2 + "}"
        end

        finalProductionString = "const float* params=reinterpret_cast<const float*>(input+1);\n\n"
        if not trivialConditionals
          finalProductionString += tabSpace*2 + "if(input[0].isInvalid()){ output[0].mID=#{@symbolHash[p]}; memcpy(output+1,input,sizeof(SymbolEntry)*#{@numProductionParams[p]+1}); logProbability=0; return #{@numProductionParams[p]+2}; }\n\n"
          finalProductionString += tabSpace*2 + "#{condLine.chop.chop}\n" + tabSpace*2 + "};\n\n"
          finalProductionString += tabSpace*2 + "#{condSumLine.chop.chop}\n" + tabSpace*2 + "};\n\n"
        else
          finalProductionString += tabSpace*2 + "#{sumLine.chop.chop}\n" + tabSpace*2 + "};\n\n"
        end
        finalProductionString += tabSpace*2 +"float probSum=0;\n" + tabSpace*2 + "for(uint i=0;i<#{rest.size};i++) probSum+=prob[i]; assert(probSum>0);\n"
        finalProductionString += tabSpace*2 + "for(uint i=0;i<#{rest.size};i++) prob[i]/=probSum;\n\n" + tabSpace*2 + "#{selectorLine.chop});\n\n"
        finalProductionString += productionString + tabSpace*2 + "}\n" + tabSpace*2 + "return -1;"
        productionString = finalProductionString
      end

      symbolFileContents.gsub!("%MAXSYMBOLS%", numSymbols.max.to_s)
      symbolFileContents.gsub!("%ISVALID%",isValidString)
      symbolFileContents.gsub!("%MAXSYMBOLENTRIES%", numSymbolEntries.max.to_s)
      symbolFileContents.gsub!("%NUMFORMALPARAMS%", @numProductionParams[p].to_s)
      symbolFileContents.gsub!("%PRODUCTIONS%", productionString)

      symbolFile.write(symbolFileContents)
      symbolFile.close
    end

    @terminals.each do |t,numParams|
      @includesList << t + "Symbol.hpp"

      symbolFile = File.open(targetDirectory+t+"Symbol.hpp","w")
      symbolFileContents = stubFileContents.dup

      symbolFileContents.gsub!("%TERMINAL%","true")
      symbolFileContents.gsub!("%SYMBOLNAME%",t)
      symbolFileContents.gsub!("%HEADERGUARD%",t.upcase)
      symbolFileContents.gsub!("%ISVALID%","return SymbolEntry::DETERMINISTIC;")
      symbolFileContents.gsub!("%RENDER%",savedRenderMethods[t] ? savedRenderMethods[t] : "")
      symbolFileContents.gsub!("%INITGRAPHICS%",savedInitGraphics[t] ? savedInitGraphics[t] : "")
      symbolFileContents.gsub!("%SAVETOMESH%",savedSaveToMesh[t] ? savedSaveToMesh[t] : "")
      symbolFileContents.gsub!("%VOXEL%",savedVoxel[t] ? savedVoxel[t] : "")

      productionString = "output[0].mID=#{@symbolHash[t]};\n"
      if numParams>0 then productionString += tabSpace*2 + "memcpy(output+1,input,sizeof(SymbolEntry)*#{numParams});\n"; end

      productionString += tabSpace*2 + "logProbability=0;\n"
      productionString +=  tabSpace*2 + "return #{numParams+1};";

      symbolFileContents.gsub!("%MAXSYMBOLS%", "1")
      symbolFileContents.gsub!("%MAXSYMBOLENTRIES%", (numParams+1).to_s)
      symbolFileContents.gsub!("%NUMFORMALPARAMS%", numParams.to_s)
      symbolFileContents.gsub!("%PRODUCTIONS%", productionString)
      symbolFileContents.gsub!("%INCLUDES%",savedIncludes[t] ? savedIncludes[t] : "")

      symbolFile.write(symbolFileContents)
      symbolFile.close
    end

    @convertedDistTerminals.each do |symbol,extensions|
      @turtleSymbols.each do |turtleSymbol,numParams,name|
        if symbol == turtleSymbol
          @convertedDistTerminals.delete(symbol)
          @convertedDistTerminals[name] = extensions
        end
      end
    end

    @convertedDistTerminals.each do |symbol,extensions|
      extIndex = 0
      extensions.each do |symbolNumber, renderParams, diffusionParams, numStoredDiffusionParams, numFormalParams|
        @includesList << symbol + "_#{extIndex}Symbol.hpp"

        symbolFile = File.open(targetDirectory+symbol+"_#{extIndex}Symbol.hpp","w")
        symbolFileContents = distTerminalStubFileContents.dup

        symbolFileContents.gsub!("%SYMBOLNAME%",symbol)
        symbolFileContents.gsub!("%HEADERGUARD%",symbol.upcase)
        symbolFileContents.gsub!("%ISVALID%","return SymbolEntry::DETERMINISTIC;")
        symbolFileContents.gsub!("%EXTENSION%",extIndex.to_s)

        productionString = "output[0].mID=#{symbolNumber};\n"
        numParams = numFormalParams + numStoredDiffusionParams
        if numParams>0 then productionString += tabSpace*2 + "memcpy(output+1,input,sizeof(SymbolEntry)*#{numParams});\n"; end
        productionString += tabSpace*2 + "logProbability=0;\n"
        productionString += tabSpace*2 + "return #{numParams+1};";

        newParamsString = "float newParams[#{renderParams.size}]={\n"
        renderParams.each { |r| newParamsString += tabSpace*3 + "#{r},\n" }
        newParamsString = newParamsString[0..-3] + "};\n" + tabSpace*2 + "#{symbol}Symbol::"

        probString = ""
        if diffusionParams.size == 1
          probString = "return log(#{diffusionParams[0][0].gsub("Sample","Eval")});"
        else
          probString = "float logProbability=0;\n";
          diffusionParams.each_with_index do |p,index|
            p = p[0].gsub("Sample","Eval")
            probString += tabSpace*2 + "logProbability+=log(#{p});\n"
          end
          probString += tabSpace*2 + "return logProbability;"
        end

        symbolFileContents.gsub!("%MAXSYMBOLS%","1")
        symbolFileContents.gsub!("%MAXSYMBOLENTRIES%",(numParams+1).to_s)
        symbolFileContents.gsub!("%NUMDIFFUSIONPARAMS%",numStoredDiffusionParams.to_s)
        symbolFileContents.gsub!("%NUMFORMALPARAMS%",numFormalParams.to_s)
        symbolFileContents.gsub!("%PRODUCTIONS%",productionString)
        symbolFileContents.gsub!("%RENDER%",newParamsString+"render(newParams);")
        symbolFileContents.gsub!("%SAVETOMESH%",newParamsString+"saveToMesh(newParams);")
        symbolFileContents.gsub!("%VOXEL%",newParamsString+"voxel(newParams);")
        symbolFileContents.gsub!("%LOGPROBABILITY%",probString)

        symbolFile.write(symbolFileContents)
        symbolFile.close
        extIndex += 1
      end
    end

    exportedFile = File.open(targetDirectory+"#{fileName}.cpp","w")
    exportedFileContents=File.open("Resources/ExportStub").readlines.join

    includesString = ""
    @includesList.each { |i| includesString += "#include \"#{i}\"\n" }
    includesString.chop!

    symbolsString = ""
    @nonTerminals.each{ |n| symbolsString +="table[#{@symbolHash[n[0]]}]=new #{n[0]}Symbol(table);\n#{tabSpace}" }
    @terminals.each{ |n| symbolsString +="table[#{@symbolHash[n[0]]}]=new #{n[0]}Symbol(table);\n#{tabSpace}" }
    @convertedDistTerminals.each do |symbol,extensions|
      index = 0
      extensions.each { |s| symbolsString +="table[#{s[0]}]=new #{symbol}_#{index}Symbol(table);\n#{tabSpace}"; index += 1 }
    end

    symbolsString = symbolsString[0..-6]

    axiomString = ""
    index = 0
    validIndex = 0
    @axiom.each do |s|
      axiomString += "axiom[#{index}].mID=#{s[0]};\n#{tabSpace}"
      if @nonTerminalHash[s[0]] then validIndex=index=index+1 end
      [*1..s[1].size].each { |i| axiomString += "axiom[#{index+i}].mParam=#{s[1][i-1]};\n#{tabSpace}" }
      index += s[1].size
      [*1..s[2].size].each { |i| axiomString += "axiom[#{index+i}].mParam=#{s[2][i-1]};\n#{tabSpace}" }
      index += s[2].size + 1
      if @nonTerminalHash[s[0]] then axiomString += "axiom[#{validIndex}].mFlags=symbolTable[#{s[0]}]->setFlags(reinterpret_cast<float*>(axiom+#{validIndex+1}));\n#{tabSpace}" end
    end
    axiomString = axiomString[0..-6]

    if @fullFilePath.include?("Architecture")
      exportedFileContents.gsub!("%MESH%","resourceHandler->initOutputMesh(new BuildingMesh(), BUILDING);")
      includesString += "\n#include \"BuildingMesh.hpp\"\n"
    elsif @fullFilePath.include?("Trees")
      exportedFileContents.gsub!("%MESH%","resourceHandler->initOutputMesh(new TreeMesh(), TREE);")
      includesString += "\n#include \"TreeMesh.hpp\"\n"
    elsif @fullFilePath.include?("Cities")
      exportedFileContents.gsub!("%MESH%","resourceHandler->initOutputMesh(new CityMesh(), CITY);")
      includesString += "\n#include \"CityMesh.hpp\"\n"
    else
      exportedFileContents.gsub!("%MESH%","resourceHandler->initOutputMesh(new Mesh(), MESH);")
    end

    exportedFileContents.gsub!("%NUMSYMBOLSINAXIOM%",@axiom.size.to_s)
    exportedFileContents.gsub!("%NUMSYMBOLENTRIESINAXIOM%",index.to_s)
    exportedFileContents.gsub!("%DERIVATIONDEPTHLIMIT%",@derivationLength)
    exportedFileContents.gsub!("%AXIOM%",axiomString)
    exportedFileContents.gsub!("%TABLESIZE%",(@tableOffset*4).to_s)
    exportedFileContents.gsub!("%SYMBOLS%",symbolsString)
    exportedFileContents.gsub!("%NUMSYMBOLS%",numSymbols.to_s)
    exportedFileContents.gsub!("%INCLUDES%",includesString)
    exportedFile.write(exportedFileContents)
    exportedFile.close
  end

  def deepCopy(s)
    Marshal.load(Marshal.dump(s))
  end

end
