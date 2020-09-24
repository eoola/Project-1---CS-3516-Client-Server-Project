SERVPORT=9500
DESTURL=www.google.com
SERVERURL=www.linux.wpi.edu/TMDG.html

runclientonly:
	./client $(URL) 80

runandcompileclientonly: clean compileclient runclientonly

runandcompileclientserver: clean compileclient compileserver runserver runclientwithserver

runserver:
    ./server $(SERVPORT)

runclientwithserver:
    ./client $(SERVERURL) $(SERVPORT)

compileclient:
    gcc -o client client.c

clean:
    rm client


