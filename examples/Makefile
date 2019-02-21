CC = gcc

all: insert-sort.c quick-sort.c merge-sort.c
	$(CC) -o insert-sort insert-sort.c
	$(CC) -o quick-sort quick-sort.c
	$(CC) -o merge-sort merge-sort.c

test: insert-sort quick-sort merge-sort
	@./insert-sort
	@./quick-sort
	@./merge-sort

.PHONY: clean
clean:
	rm insert-sort quick-sort merge-sort