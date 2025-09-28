# 🗝️ Distributed Key-Value Store over TCP/IP

A simple **TCP/IP client-server project in C++** that demonstrates socket programming, concurrent client handling, and in-memory key-value storage.  

## 📌 Features
- **Client-Server architecture** using **TCP/IP**.  
- Supports multiple concurrent clients via `fork()`.  
- Simple commands:  
  - `SET key value` → Store key-value pair in memory.  
  - `GET key` → Retrieve value for a given key.  
  - `EXIT` → Close client connection.  
- **In-memory store** (like a tiny Redis).  
- Runs on **Linux/Unix systems**.  



