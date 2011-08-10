class Distribution

  attr_accessor :name, :boundsArray

  def initialize(name,boundsArray)
    @name = name
    @boundsArray = boundsArray
  end

end

class ParamArray

  attr_accessor :name, :indexExpression

  def initialize(name,indexExpression)
    @name = name
    @indexExpression = indexExpression
  end

end

class Production

  attr_accessor :predecessor, :conditional, :probability, :successor

  def initialize(predecessor,conditional,probability,successor)
    @predecessor=predecessor
    @conditional=conditional
    @probability=probability
    @successor=successor
  end

end
