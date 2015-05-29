#Pebble Controller

Pebble Controller is a pebble app that allows you to use your pebble as
a controller for your pc. The app prompts you for a destination IP address and
sends information on each button press to a server running on port 5000 at that
IP. Currently, the server simply executes a script hook that simulates mac key
presses, but changing these scripts (or the server itself) would easily allow
this to be used on any os.

##Setup

1. Clone the repo

2. Install pebble sdk (or use cloudpebble, I think thats possible)

3. Build the app and install it on your pebble 

4. Modify the handler scripts `server/handlers` to your heart's content 

5. Start the server with:

``` 

cd server
ruby server.rb

```

6. Run the app and enter your desired ip address

7. Good to go!

##Screenshot

![screenshot](screenshot.png)
