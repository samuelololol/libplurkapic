#libfinchurk

##Requirements:

* libfinchurk, plurk api in C
* test drive: main.c
* based on [liboauth-0.9.7](http://liboauth.sourceforge.net)
* [json-c](https://github.com/json-c/json-c/wiki)

##HOWTO:


* export 2 [plurk API key](http://www.plurk.com/API) variables to your environment 

* (PKEY, PSECRET) = (API key, API secret key)

        export PKEY=xxxxx 
        export PSECRET=xxxxx 

* make

* ./go.sh

* copy the link to web browser to get verify code(yes, it's ugly)

* paste to console and press enter(still, ugly)

* check the messge on plurk :(

##TODO:

* fix the ugly part of verification to automatic process

* wrap the rest plurk APIs to C-SDK
