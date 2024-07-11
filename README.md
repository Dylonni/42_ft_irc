# 42_ft_irc
A basic IRC server, made in C++ where you can connect and chat with other connected people, create channels, moderate them and more...

>[!NOTE]
>For a better user experience, you might want to install an IRC client. We made this project with the ```irssi``` client.
>This tutorial will not explain on how to get the irssi client, as it can differ for different OS.
>This build has been made for ```irssi v1.2.3-1ubuntu4``` on an ```Ubuntu 22.04``` machine.

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
| `/join <channel>` | Will join or create a channel with the entered channel name |
| `/part <channel>` | Leave the specified channel |
| `/invite <user> <channel>` | Invite the specified user to the specified channel |
| `/kick <user>` | Kick the specified user out of the channel, you have to be a channel operator |
| `/topic <topic>` | Moify the topic (description) of the channel, channel has to be in +t mode and you have to be a channel operator |
| `/msg <channel,user>` | Send a message to specified channel or user, you have to be inside of the channel for your message to be delivered |
| `/mode <+-> <i,t,k,o,l>` | Set a mode for the channel, you have to be a channel operator to use these | </br>

| Modes | Description |
| --- | --- |
| `/mode <+-i>` | Set or remove the Invite Only mode for the channel, users can enter the channel only if they are invited |
| `/mode <+-t>` | Set or remove the ability for channel operators to modify the topic of the channel, they have to use ```/topic``` to modify it afterwards |
| `/mode <+-k> <password>` | Set a password to join the channel, you have to specify the password after setting +k (i.e ```/mode +k <password>```). If you use -k, channel password is removed |
| `/mode <+-o> <user>` | Set or remove channel operator privilege for the specified user, |
| `/mode <+-l> <limit>` | Set or remove the limit of channel users, you have to specify the limit after setting +l (i.e ```/mode +l <limit>```). If you use -l, channel users limit is removed  |

## Documentation
You can find further explanation of an IRC client protocol implementation by following this link </br>
https://modern.ircdocs.horse/ </br>

>[!TIP]
>You can also check on different RFC IRC protocols to know how to implement it correctly, just know that there are different versions of RFC and some features are not implemented the same way between on and another version
