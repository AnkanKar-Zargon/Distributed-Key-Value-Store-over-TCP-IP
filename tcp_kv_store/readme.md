## Run Instructions:

### Start the server: 
```bash 
./server
```
### Start a client (in another terminal): 
```bash 
./client
```
You can run multiple clients simultaneously. 

### Example Session: 
#### Client 1: Connected to server. Use commands: 
```bash 
SET key value | GET key | EXIT 
> SET foo bar 
OK 
> GET foo 
bar 
> GET baz 
NOT_FOUND 
> EXIT 
BYE 
```
#### Client 2 (same time): Connected to server. Use commands: 
```bash
SET key value | GET key | EXIT 
> GET foo 
bar
```