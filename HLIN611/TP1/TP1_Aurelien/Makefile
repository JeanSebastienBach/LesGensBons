########################################
#~ définitions
########################################

BIN= obj/fonc.o  bin/client  bin/serveur

# liste des fichiers sources
SRCS0=client.c
SRCS1=serveur.c
SRCS2=fonc.c

default: $(BIN)

########################################
#~ regles pour l'executable
########################################

obj/%.o: %.c
	gcc -Wall -Iinclude -c $< -o $@

bin/client: $(SRCS0:%.c=obj/%.o) $(SRCS2:%.c=obj/%.o)
	gcc -lpthread -o $@ $+

bin/serveur: $(SRCS1:%.c=obj/%.o) $(SRCS2:%.c=obj/%.o)
	gcc -lpthread -o $@ $+

clean:
	rm -f $(BIN) obj/*.o *~
