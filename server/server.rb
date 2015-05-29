require 'sinatra'

configure do
  set :port, 5000
  set :bind, '0.0.0.0'
end

post '/data' do
  puts params['button']
  status 200


  puts Dir.pwd
  case params['button'].to_i
  when 0
    `handlers/back.sh &`
  when 1
    `handlers/up.sh &`
  when 2
    `handlers/select.sh &`
  when 3
    `handlers/down.sh &`
  end
end

