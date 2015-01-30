Communication protocols for Neurohex
====================================

Introduction
------------

The Neurohex application consists of three programs: a client, a server and a
metaserver. A typical session with the application will consist of the client
connecting to a metaserver, communicating with it for a while, then connecting
to a server specified by the metaserver, playing a game and connecting back to
the metaserver. The specifics of the neccessary communication are detailed
below. Note that the protocols are not yet fully implemented.

Prerequisites
-------------

All the communication is send in the form of series of DFStyle tokens. Tokens
consist of a token name and optionally some values. A token is represented as a
string beginning with a '[', followed by the token name in allcaps, optionally
followed by a series of values and ending with an ']'. All the values must be
strings preceded by a ':'. Token names ending with 'BEGIN' signal the beginning
of a token group, which may contain some tokens and should end with a token with
the same prefix as the beginning one, but ending with 'END'. A message is a
string consisting of any number of tokens terminated by the
sequence '\r\n\r\n'.

Libraries for handling DFStyle tokens are in the utility namespace and their
names begin with DFStyle, see the project technical documentation for details.

This style of tokens was inspired by configuration files of the brilliant game
Slaves to Armok: God of Blood Chapter II: Dwarf Fortress and was chosen for easy
extensibility.

Client-metaserver communication
-------------------------------

### Joining the server

The metaserver waits for connections on a specified port, 14522 by default.
After accepting a connection the metaserver waits for the client to send a AUTH
token, containing a nick and a key identifying the client (which shall be
generated when first running a new installation of the client). If the key is
different than the keys of all already authenticated users, the client becomes
an authenticated user himself. A nick is assigned to the player equal to the
nick requested if it is not already used, if it is, a nick with a random number
appended is assigned. Otherwise the old user is assumed to be a dead connection
and dropped, while this one inherits its nick. In both cases a single WELCOME
token is send back, containing the assigned nick.

### Pre-game chat

Afterwards the server awaits communication from the client, which might send one
of the following messages:

* A single CHAT token with two values, a destination and a chat line.
		The destination is either a group name or	empty. If the destination is a
		non-existent group name a NO_GROUP token is send back. Otherwise the chat line
		is written to the proper chat group or general chat if the destination is
		empty and a SENT token is send back.
* A single MAKE_GROUP token with one value, a name. If no group with the given
		name exists it gets created with the sender of the message as its owner and
		sends a single GROUP_CREATED token back. If such a group exists a single
		GROUP_EXISTS token is send back.
* A single JOIN_GROUP token with one value, a name. If no group with the given
		name exists a NO_GROUP token is send back. Otherwise the user is added to the
		group, possibly leaving his old group if he was in one, and a JOINED token is
		send back.

At the same time the server might send CHAT tokens analogous to the ones above
updating chat. The client should respond with a RECEIVED token to every one of
these.

### Initiating a game

The owner of a group might send a INITIATE_GAME token, containing values with nicks
of users in the group, including the sender. If not all of the users specified
in the request are in the group a PLAYERS_MISSING token is send back. Otherwise
all other users are removed from the group, while the ones specified get a
GAME_INITIATED token with the name of the group. If they all send back a
ACCEPT_GAME token containing the group name we move to the server lookup phase.
Otherwise (if a timeout is reached or a DECLINE_GAME token with the group name
is send) all the users get a GAME_INITIATION_ABORTED token with the group name.

### Server lookup

Most of this phase can be skipped by starting a server on the same machine as
the metaserver. This is obviously not desirable in many cases so we can
negotiate starting a server remotely on a client machine. The metaserver can
send a SERVER_REQUEST token to a client, to which the client replies either with
a ACCEPT_SERVING or DECLINE_SERVING token. This will be tried until a client replies
with the former. After the client has agreed to become a server a START_SERVING
token is send by the metaserver and the client replies with a SERVER_STARTED
token with two values, an address and a port at which the server has been
started.

### Server transfer

At this point the metaserver communicates with the server (see Metaserver-server
communication) and after acquiring proper information sends all the clients a
SERVER_TRANSFER token with two values, the address and port at which the new
server is listening. If this communication failed we go back to the server
lookup phase. The clients connect to the server (see Client-server
communication) and send a TRANSFER_COMPLETED token back. Any client that cannot
connect to the server should send a TRANSFER_FAILED token. If any transfer
failed the server sends a ABORT_TRANSFER token to all clients in the game and we
go back to server lookup. Otherwise the metaserver transfers a
TRANSFER_COMPLETED to all the clients and closes its connection to them.

Metaserver-server communication
-------------------------------

After a server has been started, either by the metaserver or by a client at the
request of the metaserver the metaserver connects to it and sends a message with
a METASERVER token, followed by a USERS token containing keys of users supposed
to join the game, the first of which is the group owner. The server responds
with a BEGIN_TRANSFER token -- this is the point at which the metaserver tells
the clients to connect to the server. After all the specified users successfully
connect the server sends a TRANSFER_COMPLETE token and both the servers close
their connections. If any user fails to connect and reports this to the
metaserver, the metaserver sends a ABORT_TRANSFER token to the server, which
should then shut down.

Client-server communication
---------------------------

### Server setup

After the server gets started it waits for the first connection at some port,
14521 by default. This connection can either be the metaserver connection (see
Metaserver-server communication) or an ordinary user connection. In the former
case the server becomes a private server and will deny connections from users
not specified in the metaserver message. In the latter case the server is a
public server and will accept any users. This is to ensure that standalone game
servers can be set up by users not willing to connect to metaservers.

The servers might also have other than the metaserver message reasons to decline
specific connections, e.g. bans or just accepting specified people. This is up
to the specific implementation of the server.

### Joining the server and authentication

When a client connects to a server it should send a AUTH token with a nick and
key authenticating the client. If the server accepts the connection it will send
a WELCOME token back, otherwise it sends a DECLINED token with an optional
explanation string in it. The server will decline any connection from clients
sending a key already in use by a connected client.

If the server loses the connection to an already authenticated client at any
point, it will accept connections that send an AUTH token containing the key of
a disconnected user and treat them as the connections from the user. Any other
connections are declined.

### Choosing the game admin

The game admin is either specified in the metaserver message as the owner of the
first key send by the metaserver (see Metaserver-server communications) if the
server is private or this role is given to the first client that connects.

### Game configuration and start

Some configuration may be required before the game starts. The server sends
messages to the game admin starting with a REQUEST token containing the name of
the request and optionally followed by a list of options in the form of a
OPTIONS token group containing OPTION tokens with two values each -- the first
being the name of the option and the second being a short description. The game
admin should respond with a single CHOICE token containing the name of the
request and the chosen option form the list or, if no list was provided, any
legal value. A list of currently allowed configuration requests with legal
responses follows.

* initialHealth -- If no options are provided any positive integer is a legal
		response.
* board -- Options must be provided.

This list is subject to change, in particular choosing a game mode or rule
modification sets should be possible.

Before the game starts all users should choose their armies. This is also
requested by the server in a message starting with a REQUEST token containing
the string 'army' followed by a list of options. Similarly as above the client
responds with a CHOICE token containing the request name and the name of the
army chosen.

If any of the above choices are invalid, the server send an INVALID_CHOICE token
containing the request name and might try again.

After the configuration is complete the server sends a GAME_STARTING token
followed by a description of the game state (see below).

### Sending game objects

All the game objects are sent as token groups generated by their encodeAsDFStyle
methods and decoded by their fillFromDFStyle methods. The specifics of this
process are not subject to this documentation, but since both the client and
server use the same underlaying game objects those objects just need to be
internally consistent.

The only possibly interesting part is sending nonpublic information. Luckily the
only such information in the game is the order in which tiles will be drawn,
which does not have to be sent at all, though the current implementation sends
it after randomizing it.

### Update requests

At any point any client might send a REQUEST token containing the string
'gameState' to request the whole game state to be transferred to it. To this the
server should respond by sending a UPDATE token followed by the whole game
object. This is mostly useful after losing the connection.

### Player actions

The server sends requests to clients when player choice is needed. Those
requests have the form of REQUEST tokens containing the name of the request,
optionally an additional value and possibly followed by some game objects
specifying the request. The response should start with a CHOICE token containing
the name of the request and possibly another value depending on the request.
This token is optionally followed by some game objects, also depending on the
request. A list of currently allowed requests follows.

* move -- This request is followed by a PLAYERID token containing the id of the
		player to whom the request is send and a game object. The response should
		consist of the standard choice token followed by a move object.
* targets -- This request contains the number of abilities for which the player
		should choose targets. It is followed by the id of the player to whom the
		request is send, a game object and the specified number of ability objects.
		The response should consist of the standard choice token followed by as many
		targets objects as abilities were requested.
* discard -- This request is followed by the id of the player to whom the
		request is send and a game object. The response should consist of a choice
		token containing one number, corresponding to the tile in hand the player
		wants to discard.

After receiving the response the server might send back an INVALID_CHOICE
token and try again if the response was not valid.

### Updates

The server might send updates if the game state changed at any point. This is
done just as in the Update requests section, but might happen at any point. It
is not guaranteed that this will happen at all.

### Game end

After the game ends, the server sends a GAME_FINISHED token containing the nick
of the winner. Afterwards the server shuts down.

Current implementation
======================

Currently only a proof-of-concept is implemented. Specifically only some of the
above described protocols are supported. The most important differences are
described below.

* No metaserver
* No keys, authentication only by nick
* No authentication refusal
