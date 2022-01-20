# Program documentation
This program can be used to log GNU/Linux systems.
Processes collecting logs are called agents.

You can select, which agents will be collecting in file `agents-list.conf`.
On each line must be path to folder (folder name cannot contain empty spaces), in which is agent executable with same name as the folder.
You can use `#` as a comment.

You can configure agents with their config files - settings like how often take log, which values are considered as error etc.

After start, enabled agents are created and they collect logs. These logs are saved in `logs/` folder, which is created after program termination.

## How to run
`mkdir build && cd build && cmake .. && make && ./modulog`




## Dependencies 

1. boost asio (no need for whole boost, asio is developed standalone)
2. DaanDeMeyer/reproc (reproc++)
3. nlohman/json

## Implementation TODO
    Diagram architektury a funkcnosti
    Dodelat poradne ukoncovani programu
    Pouzivat pro include knihoven <> misto uvozovek,
    bind --> function,
    MSG_TYPE do vastniho enum class
6. Log somewhere if agent crash
7. When too much agents and running with Valgrind, connection freeze and crashes - TODO fix
8. Create architecture diagram
9. Compile just enabled agents
10. Create global config file

## TODO

1. Write, why is our solution different
2. Write to doc, how to create agents

---





--- 

## Pitfalls we will work on
1. Not the most effective - like sending logs in json etc.
2. Not rotating log files
3. Not the most clean exit with CTRL+C

# END PROGRAM DOCUMENTATION (continuing just some notes)
---

---

---

---

# Implementation tasks

1. Our app will have each monitoring agent as a process, so we need to find out, how to create/stop processes.
   Agents will be executed from main app, which will be called Core.
   We can look for some light-weighted library to manage it - we have to, if library offers handle to process, if we would like to add them in process groups.
   We will call these processes agents and they will collect system info in the future.
2. We want our agents to communicate with core app via TCP/IP on localhost. When agent fails, we need to know it. We can use some library, which doesn't have too much dependencies and is not too large (look at boost::asio / POCO project).
3. We need to send various messege types like msg::log, msg::config or msg::log. We have to check, if agent is not freezed with sending control messages and agent has to respond to it. We will also have to think about communication protocol between agent and core.
   Agent will also automatically send some random log messages.
4. We want to create two agents - one to monitor temperature, second to monitor internet connection.
   We will write and use config files for these agents (like how often send logs, what is max temperature etc.).
   Agents will then collect logs, send them to core app and core app will save these logs in folder.
   If agent sends some error message, core app will point on it from general log file.

---

# Already existing tools - research

### Zabbix
Cons:
1. hard configuration - takes time to all setup - have to install and configure server, database and agents 
1. hard to use - reviews tell, that documentation is bad and you have to use it for long time to understand it
1. uses database to store configuration and data - more difficult to manipulate with it

### Nagios
Cons:
1. Big system requirements: 20 GB of free hard drive space, 2 GB memory
1. hard to configure

### Munin
Cons:
1. set time interval between logs?
1. key-value plugin?
1. mainly for visualisation in graphs - cannot sort messages like DEBUG, INFO, ERR. and cannot set watchdogs (if is value > x - tell me about it in some general log file)

### Datadog
Cons:
1. paid

---

# Which libraries to use - research

### Process handling libraries
**sheredom/subprocess.h**

- Is header-only
- No ability to send SIGTERM
- No error if starting not existing script

**eidheim/tiny-process-library**

- Last commit 10 months ago - not too much updated
- doesn't look too profi

**boost, POCO**

- too huge

**DaanDeMeyer/reproc**

- seems pretty good





