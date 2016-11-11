# CS460_Daytime

Project made by Bruno TESSIER and Tyler TOLLEFSON

Simple daytime client in java to ask specific part of the time given by a server in C which ask to a NIST server

To install : make

To launch server in c: ./server

To launch client in java: java Client ip_adress port

Protocol between Client in c and server in java:

  Client ask for a part of the time (1 char):   - '0' = Year requested
                                                - '1' = Month requested
                                                - '2' = Day requested
                                                - '3' = Hours requested
                                                - '4' = Minutes requested
                                                - '5' = Seconds requested

  Server send back the requested data :         -If time updated by NIST server : (3char) 2char for data + end of line char
                                                -Error message : If didn't manage to get the time
