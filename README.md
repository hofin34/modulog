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

# Cross-compiling
* when program throws error "GLIBCXX_3.4.26 not found" you have to update libstdc++6

## Raspberry Pi 4
* install arm-linux-gnueabihf (version 9.3.0 working on RPI OS Bullseye having GCC 10.2.1)
* copy /lib, /opt, /usr folders from Rpi to local folder (in our case called "rootfs")
* through cmd or in file `rpi-toolchain-file.cmake` specify path to this folder (variable `CMAKE_FIND_ROOT_PATH` and `CMAKE_SYS_ROOT`)
* now you can build:
   * `mkdir build`
   * `cd build`
   * `cmake -DCMAKE_TOOLCHAIN_FILE=../toolchains/raspberry_toolchain.cmake -DCMAKE_INSTALL_PREFIX="someDir" ..`
   * `make -j 8 install`
   * output in `someDir` can be moved to RPI

## aarch64 architecture
* install `gcc-aarch64-linux-gnu` (tested version 9.3.0)
* rest same as above, just modify cmake to `-DCMAKE_TOOLCHAIN_FILE=../toolchains/aarch_toolchain.cmake`

## Implementation TODO
* Log somewhere if agent crash 
* When too much agents and running with Valgrind, connection freeze and crashes - TODO fix 
* Compile just enabled agents

## TODO
* Write to doc, how to create agents


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

# Test cases
* Write speed to tests and save history
* Test communication library - sending big logs / big frequency
* add return codes for different exits:
  * Agent fails to start
  * Couldn't open file
  * Runtime err
* If agent fails during startup
* If agent asks to exit
* if is agent killed
* If all agents are killed - should exit with 0
* if all agents asks to exit
* if agent freezes - will it kill him? (also with more agents)
* if is logging correctly? and errors?
* logging with a lot of agents and big log frequency
* Test it with valgrind?
* 


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


---

## Osnova BP
# Teoretická část
## Napsat požadavky firmy
## Výběr technologií 
## Proč existující nevyhovují + větší porovnání
## Meziprocesní komunikace - výhody/nevýhody + proč TCP nejlepší
## 

# Praktická část
## Diagramy + vysvětlení principu
## Popis struktury projektu???
## Testování - real + GTesty
## Analýza výkonu
