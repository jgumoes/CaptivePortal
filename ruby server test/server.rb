# require 'sinatra'
require 'sinatra/base'
require_relative 'libs/ServerVariables'
require_relative 'libs/functions'

class Portal < Sinatra::Base

    enable :sessions
    
#     def initialize
#         @connected = true
#         @wrong_pass = false
#     end
    before do
        @vars = ServerVariables.instance()
    end

    get "/" do
        @vars = ServerVariables.create()
        @device_name_change = device_name_change(@vars)

        @connection_status = connection_status(@vars)
        @wrong_password = test_password(@vars)
        @wifi_ssid = scan_networks(@vars)

        erb :index
    end
    # # start the server if ruby file executed directly
    run! if app_file == $0
end
