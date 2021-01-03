# Make individual makefiles using "make filename" (no file suffix) or all using "make all". 
# Remove all using "make clean"

TARGET = prioRW producer-consumer pthread-creation pthread-creation-quiz-1 pthread-creation-quiz-2 pthread-creation-quiz-3

all: prioRW producer-consumer pthread-creation pthread-creation-quiz-1 pthread-creation-quiz-2 pthread-creation-quiz-3

prioRW: prioRW.c
	gcc -g -Wall -o prioRW prioRW.c -lpthread

producer-consumer: producer-consumer.c
	gcc -g -Wall -o producer-consumer producer-consumer.c -lpthread

pthread-creation: pthread-creation.c
	gcc -g -Wall -o pthread-creation pthread-creation.c -lpthread

pthread-creation-quiz-1: pthread-creation-quiz-1.c
	gcc -g -Wall -o pthread-creation-quiz-1 pthread-creation-quiz-1.c -lpthread

pthread-creation-quiz-2: pthread-creation-quiz-2.c
	gcc -g -Wall -o pthread-creation-quiz-2 pthread-creation-quiz-2.c -lpthread

pthread-creation-quiz-3: pthread-creation-quiz-3.c
	gcc -g -Wall -o pthread-creation-quiz-3 pthread-creation-quiz-3.c -lpthread

clean:
			$(RM) $(TARGET)