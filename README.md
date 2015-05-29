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
bundle install
ruby server.rb

```

6. Run the app and enter your desired ip address

7. Good to go!

##Suggested Uses

This was originally developed as a method to control presentations with
a pebble and was once tied to [RevealJS](https://github.com/hakimel/reveal.js).
It works well for this use case, so go ahead and test it out
[here](http://lab.hakim.se/reveal-js/#/). 

Now it has become more flexible so use it for whatever you wish.

##Screenshot

![screenshot](screenshot.png)
