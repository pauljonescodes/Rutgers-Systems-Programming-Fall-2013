mbook = gcc book.c book.h tokenizer.c tokenizer.h -o book
mconsumer = gcc book.h consumer.c -o consumer

all:
	$(mbook)
	$(mconsumer)

book:
	$(mbook)
	
consumer:
	$(mconsumer)
	
clean:
	rm book
	rm consumer
