
# 💻 Remote Command Execution (Client-Server) Project

This project implements a **simple remote command execution system using C++ and Winsock on Windows**.
It consists of two programs: a **server** and a **client**. The client connects to the server over TCP, sends shell commands, and receives their output. The server executes these commands using `cmd.exe` and returns the result.

---

## ⚙️ Features

* Connect to a server using IP and port entered at runtime
* Send any Windows command (e.g., `dir`, `mkdir`, `echo`, `type`, etc.)
* Receive full command output from the server
* Create folders and files remotely
* Edit and read file contents remotely
* Supports continuous command execution until `exit` is typed
* ✅ **Supports multiple clients simultaneously (multi-threaded server)**

---

## 🗂️ File Structure

```
NP_Project/
├── client.cpp
├── server.cpp
├── README.md
```

---

## 🚀 Build Instructions

### 🖥️ Requirements

* Windows operating system
* Microsoft Visual Studio or MSVC compiler (cl.exe)
* Winsock library (linked by default with `Ws2_32.lib`)

### 🧑‍💻 Compile

#### Server

```powershell
cl server.cpp /EHsc /Fe:server.exe /link Ws2_32.lib
```

#### Client

```powershell
cl client.cpp /EHsc /Fe:client.exe /link Ws2_32.lib
```

---

## 💬 Usage

### Start server

```powershell
.\server.exe
```

The server starts listening on port **5500** and waits for incoming connections.
**Multiple clients can connect at the same time, each handled in a separate thread.**

---

### Start client

```powershell
.\client.exe
```

You will be prompted to enter:

* **Server IP address** (e.g., 192.168.1.100)
* **Server port** (e.g., 5500)

Then, you can type shell commands (or `exit` to quit).

---

## 💡 Example Commands

```cmd
whoami
hostname
mkdir myfolder
echo Hello World > myfolder\file1.txt
type myfolder\file1.txt
echo Another line >> myfolder\file1.txt
type myfolder\file1.txt
dir
systeminfo
cd myfolder && echo Inside folder > innerfile.txt
type myfolder\innerfile.txt
```

---

## ⚠️ Important Notes

* Commands run in the context of the **server system** (use carefully!).
* The server supports **multiple clients simultaneously** using threads.
* Firewall rules might need to be adjusted to allow TCP connections on port 5500.
* `cmd.exe /c` is used, so any Windows shell command should work as if typed in a normal CMD window.
* Make sure to properly close each client by typing `exit` to avoid leaving threads hanging.
