class ServerVariables
  attr_accessor :connected, :wrong_pass, :networks, :device_name, :new_name, :network_name
  def initialize(connected = true, wrong_pass = false)
      @connected = connected
      @wrong_pass = wrong_pass
      @networks = ["FBI Van number 2", "Stupid Starbucks", "Uncle Touchy's Mystery Van"]
      @device_name = "Butt's McGee"
      @new_name = false
      @network_name = ""
  end

  def self.create(connected = true, wrong_pass = false)
      @vars = ServerVariables.new(connected, wrong_pass)
  end

  def self.instance
      @vars
  end
end