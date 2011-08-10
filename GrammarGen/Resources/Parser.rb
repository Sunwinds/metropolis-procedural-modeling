#!/usr/bin/ruby1.9

#############################################################
# Includes                                                  #
#############################################################
require 'rbconfig'
require 'fileutils'
require 'rubygems'
require 'polyglot'
require 'treetop'
require 'Resources/LSystem'
require 'Resources/Grammar'

class Parser

  #############################################################
  # Change at your own risk!                                  #
  #############################################################
  TurtleSymbols = [ ["[",0,"LeftBracket"], ["]",0,"RightBracket"], ["+",1,"Plus"], ["-",1,"Minus"], ["&",1,"Ampersand"], ["^",1,"Caret"], ["\\",1,"ForwardSlash"], ["/",1,"BackSlash"], ["|",0,"Pipe"] , ["!",1,"Exclamation"] ]
  MathSymbols = ["(",")","+","-","*","/","<","<=",">",">=","==","!=","&&","||","^","!","true","false","abs","acos","asin","atan","ceil","cos","cosh","exp","fabs","floor","log","log10","sin","sinh","sqrt","tan","tanh","?",":"]
  ProbDists = [ ["NormalSample", 2, 2], ["UniformSample", 2, 1] ]

  def initialize
    @fullFilePath
    @fileName = ""
    @targetDirectoryName = ""
    @grammarFile = []
    @grammar = []
    @baseDir = ENV['PROC_MOD_BASE_DIR']
    if !@baseDir then puts "PROC_MOD_BASE_DIR is not defined."; Process.exit end
    @sys_name = ''
    case RbConfig::CONFIG['host_os']
    when /mswin|windows|mingw32/i
      @sys_name = 'windows'
    when /linux/i
      @sys_name = 'linux'
    when /darwin/i
      @sys_name = 'mac'
    end
  end

  attr_accessor :grammar, :fileName

  def loadGrammarFile(fileName)

    @fullFilePath = fileName

    begin
      if @sys_name == "windows"
        puts "Running preprocessor for Windows " + fileName + " " + fileName + ".pp"
        curDir = Dir.getwd

        vcvarsdir = ""
        if ENV['VS80COMNTOOLS']
          vcvarsdir = ENV['VS80COMNTOOLS']
        elsif ENV['VS90COMNTOOLS']
          vcvarsdir = ENV['VS90COMNTOOLS']
        elsif ENV['VS100COMNTOOLS']
          vcvarsdir = ENV['VS100COMNTOOLS']
        elsif
          puts "Could not find a Visual Studio installation"
          Process.exit
        end
        vcvarsdir = vcvarsdir + "../../VC"

        Dir.chdir(vcvarsdir)
        vcvarsdir = Dir.getwd
        vcvarspath = "\"" + vcvarsdir + "/vcvarsall.bat\""
        command1String = vcvarspath
        Dir.chdir(curDir)

        command2String = "cl.exe /EP " + fileName + " > " + fileName + ".pp"
        command1String = command1String.gsub("/","\\")
        fullCommand = command1String + ' & ' + command2String
        system(fullCommand)
      else
        puts "Running preprocessor " + fileName + " " + fileName + ".pp"
        system("g++ -E -P -x c++ " + fileName + "  > " + fileName + ".pp")
      end
    rescue Exception
      puts "Preprocessor failed"
      Process.exit
    end

    begin
      @grammarFile = File.open("#{fileName}.pp")
      @fileName = File.basename(fileName,File.extname(fileName))
    rescue Exception
      puts "File #{fileName} could not be opened"
      Process.exit
    end

    cleanedFile = []
    multiLine = false
    @grammarFile.each_line do |line|
      if line.strip.empty? then next
      else
        if multiLine
          cleanedFile[cleanedFile.size-1] = cleanedFile[cleanedFile.size-1] + line
          if /.*;/.match(line.strip) then multiLine = false end
        else
          cleanedFile << line
          if not /.*;/.match(line.strip) then multiLine = true end
        end
      end
    end
    @grammarFile.close()
    @grammarFile = cleanedFile

    FileUtils.rm "#{fileName}.pp"
  end

  def parse
    treetopParser = LSystemParser.new
    @grammar = Grammar.new(@fullFilePath,TurtleSymbols,MathSymbols,ProbDists)

    productions = []
    arrays = []
    axiom = []
    derivationLength = "5"

    @grammarFile.each do |line|
      p = treetopParser.parse(line)
      if !p
        puts "Grammar does not parse\n\nError in:\n #{line}\n"
        Process.exit
      end

      begin
        @grammar.addNonTerminal(p.Predecessor)
        if(p.Productions) then productions << p.Productions end
        if(p.Axiom) then axiom = p.Axiom end
        if(p.DerivationLength) then derivationLength = p.DerivationLength end
        if(p.ArrayDefinition) then arrays << p.ArrayDefinition end
      rescue Exception => err
        puts "Grammar does not parse \n\nError in:\n #{line}\n"
        puts err.message
        Process.exit
      end
    end

    begin
      @grammar.addArrays(arrays)
      @grammar.addAxiom(axiom)
      @grammar.addProductions(productions)
      @grammar.setDerivationLength(derivationLength)
    rescue Exception => err
      puts "Grammar does not parse"
      puts err.message
      Process.exit
    end

    @grammar.createSymbolHash
  end

  def finalize
    begin
      @grammar.convertAxiom
      @grammar.convertProductions
    rescue Exception => err
      puts "Grammar does not parse"
      puts err.message
      Process.exit
    end

    puts "Grammar parsed successfully: #{@grammar.numSymbols} symbols"
  end

  def writeFiles(grammarName=@fileName)
    puts "\nCreating C++ classes..."

    renderMethods = {}
    includes = {}
    graphicsMethods = {}
    saveToMeshMethods = {}
    voxelMethods = {}

    @fileName=grammarName
    @targetDirectoryName = "#{@baseDir}/Code/GrammarGen/GenSources/#{grammarName}/"

    targetDirectoryEntries = []

    if not File.exists?(@targetDirectoryName)
      FileUtils.mkdir_p(@targetDirectoryName)
    else
      targetDirectory = Dir.open(@targetDirectoryName)
      targetDirectoryEntries = targetDirectory.entries
      targetDirectory.close
    end

    if targetDirectoryEntries.size > 2
      targetDirectoryEntries.each do |file|
        if file =~ /^([A-Za-z0-9]*)Symbol.hpp/
          symbolName = $~[1]
          symbolFile = File.open(@targetDirectoryName+file)
          renderMethod = ""
          inRenderMethod = false
          graphicsMethod = ""
          inGraphicsMethod = false
          include = ""
          inInclude = false
          saveToMesh = ""
          inSaveToMesh = false
          voxel = ""
          inVoxel = false
          symbolFile.each_line do |line|
            if inInclude && line == "namespace ProceduralModeling{\n" then inInclude = false end
            if inInclude then include += line end
            if line.strip == "#include \"Symbol.hpp\"" then inInclude = true; end

            if inRenderMethod && line == "    }\n" then inRenderMethod = false end
            if inRenderMethod then unless line == "    {\n" then renderMethod += line end end
            if line.strip == "virtual void render(const float* params) const" then inRenderMethod = true end

            if inGraphicsMethod && line == "    }\n" then inGraphicsMethod = false end
            if inGraphicsMethod then unless line == "    {\n" then graphicsMethod += line end end
            if line.strip == "virtual void initGraphics()" then inGraphicsMethod = true end

            if inSaveToMesh && line == "    }\n" then inSaveToMesh = false end
            if inSaveToMesh then unless line == "    {\n" then saveToMesh += line end end
            if line.strip == "virtual void saveToMesh(const float* params) const" then inSaveToMesh = true end

            if inVoxel && line == "    }\n" then inVoxel = false end
            if inVoxel then unless line == "    {\n" then voxel += line end end
            if line.strip == "virtual void voxel(const float* params) const" then inVoxel = true end
          end
          renderMethods[symbolName] = renderMethod.chomp
          graphicsMethods[symbolName] = graphicsMethod.chomp
          includes[symbolName] = include.chomp
          saveToMeshMethods[symbolName] = saveToMesh.chomp
          voxelMethods[symbolName] = voxel.chomp
          symbolFile.close
        end

        if file =~ /.*\.hpp/ then
          FileUtils.rm @targetDirectoryName+file
        end
      end
    end

    @grammar.writeSymbols(@targetDirectoryName,@fileName,renderMethods,includes,graphicsMethods,saveToMeshMethods,voxelMethods)

  end

  def build
    puts "Building plugin..."

    bootstrapFile = File.open(@targetDirectoryName+"Bootstrap","w")
    bootstrapFileContents=File.open("#{@baseDir}/Code/GrammarGen/Resources/BootstrapStub").readlines.join
    bootstrapFileContents.gsub!("%PROJECTNAME%",@fileName)
    bootstrapFile.write(bootstrapFileContents)
    bootstrapFile.close

    currentDirectory = Dir.pwd
    Dir.chdir(@targetDirectoryName)
    system("ruby Bootstrap")
    if @sys_name == "windows"
      system("nmake")
    else
      system("make")
    end
    Dir.chdir(currentDirectory)
  end

end
