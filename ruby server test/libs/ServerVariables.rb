class ServerVariables
  attr_accessor :connected, :wrong_pass, :networks, :device_name, :new_name, :network_name, :last_ssid
  def initialize(connected = false, wrong_pass = false)
    @connected = connected
    @wrong_pass = wrong_pass
    @device_name = "Butt's McGee"
    @new_name = false
    @network_name = ""
    @last_ssid = ""
  end

  def set_networks(empty=false)
    if empty
      @networks = []
    else
      @networks = ["FBI Van number 2", "Stupid Starbucks", "Uncle Touchy\'s Mystery Van"]
    end
  end
  
  def reset
    # reset the flags
    @wrong_pass = false
    @new_name = false
  end

  def check_network(network, password)
    # this is to simulate login attempts
    test_network = "Stupid Starbucks"
    test_password = "Alligator3"
    if password == test_password
      @network_name = network
      @connected = true
    else
      p network, password
      @wrong_pass = true
      @connected = false
      @network_name = ""
      @last_ssid = network
    end
  end

  class << self
    def create(connected = false, wrong_pass = false)
      @vars = ServerVariables.new(connected, wrong_pass)
    end
    
    def instance
      @vars
    end
  end
end