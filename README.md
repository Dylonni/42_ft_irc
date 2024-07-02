# 42_ft_irc
A basic IRC server, made in C++ where you can connect and chat with other connected people, create channels, moderate them and more...

>[!NOTE]
>For a better user experience, you might want to install an IRC client. We made this project with the ```irssi``` client.
>This tutorial will not explain on how to get the irssi client, as it can differ for different OS.
>This build has been made for ```irssi v1.2.3-1ubuntu4``` On an ```Ubuntu 22.04``` machine.

## Run the server
To run the server , you have to clone the repository. </br>
Inside of the repository you compile the files with ```make``` </br>
The usage of the compiled is as follows </br>
```./ircserv <SERVER_PORT> <PASSWORD>```

## Connect to the server
Once the server has started you can join it by running this command
```/connect <SERVER_IP> <SERVER_PORT> <PASSWORD> <NICKNAME>```

# Commands
| Command | Description |
| --- | --- |
| `/join <channel>` | Will join or create a channel with the entered <channel> |
| `/part <channel>` | Leave the specified <channel> |
| `/kick <user>` | Kick the specified <user> out of the channel, you have to be an operator of the channel |
| `/mode <+> or <-> <i,t,k,o,l>` |  |
| `/part <channel>` | Leave the specified <channel> |
| `/part <channel>` | Leave the specified <channel> |
| `/part <channel>` | Leave the specified <channel> |
| `/part <channel>` | Leave the specified <channel> |
