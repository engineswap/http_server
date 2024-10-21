http server in c++

very performant

todo:
make the boostbeast req obj and client socket back to local variables to allow for multi-threading 

-> means calls
startListen() 
- Connects
- Reads request
- Calls parseRequest()

parseRequest(buffer)
- turns buffer to boost beast request 
- sets m_req * 

handleRequest()
- uses m_req to send respond 
