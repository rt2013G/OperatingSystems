// Pseudocode for the bounded buffer problem
// 1. Producers cannot add to the buffer if it is full
// 2. Consumers cannot remove from the buffer if it is empty
// 3. Producers and cosumers cannot access the buffer at the same time
// 4. Buffer is a FIFO queue

// Solution 1 with busy waiting
var 
    produced : boolean;
    consumed : boolean;
begin
    produced := false;
    consumed := false;
    producer()
    {
        Parbegin
            repeat
                while produced = false
                // CS begin
                if an empty buffer exists
                then 
                    produce an item
                    produced := true;
                // CS end
            forever;
        Parend;
    }

    consumer()
    {
        Parbegin
            repeat
                while consumed = false
                // CS begin
                if a full buffer exists
                then
                    consume an item
                    consumed := true;
                // CS end
            forever
        Parend;
    }
end

// Problems:
// 1. Producer has to iterate the entire array to search for an empty slot and keep producing
// but while doing so, the consumer cannot consume.
// 2. The producer has to search for an empty slot to produce, which leads to busy waiting.


// Solution 2 with 1 producer, 1 consumer and 1 buffer
var
    buffer: array[0] of item;
    buffer_full: boolean;
    producer_blocker: boolean;
    consumer_blocker: boolean;
begin
    buffer_full := false;
    producer_blocker := false;
    consumer_blocker := false;
    producer()
    {
        Parbegin
            repeat
                check_buffer_empty;
                Produce in the buffer
                post_buffer_full
            forever
        Parend;
    }
    consumer()
    {
        Parbegin
            repeat
                check_buffer_full;
                consume from the buffer
                post_buffer_empty
            forever
        Parend;
    }
end

procedure check_buffer_empty()
{
    if buffer_full = true
    then
        producer_blocker := true;
        block(producer)
}

procedure post_buffer_empty()
{
    buffer_full := true;
    if consumer_blocker = true
    then
        consumer_blocker := false;
        activate(consumer)
}

procedure check_buffer_full()
{
    if buffer_full = false
    then
        consumer_blocker := true;
        block(consumer)
}

procedure post_buffer_full()
{
    buffer_full := false;
    if producer_blocker = true
    then
        producer_blocker := false;
        activate(producer)
}


// Solution 3 with semaphores, 1 producer, 1 consumer and 1 buffer
var
    full : Semaphore := 0;
    empty : Semaphore := 1;
    buffer : array[0] of item;
begin
    producer()
    {
        Parbegin
            repeat
                wait(empty);
                Produce in the buffer
                signal(full)
            forever
        Parend;
    }
    consumer()
    {
        Parbegin
            repeat
                wait(full);
                consume from the buffer
                signal(empty)
            forever
        Parend;
    }
end


// Solution 4 with semaphores, n producers, n consumers and n buffers
var
    buffer: array[0..N-1] of item;
    full : Semaphore := 0;
    empty : Semaphore := n;
    CS_prod : Semaphore := 1;
    CS_cons : Semaphore := 1;
    prod_ptr : integer := 0;
    cons_ptr : integer := 0;
begin
    producer()
    {
        Parbegin
            repeat
                wait(empty);
                wait(CS_prod);
                Produce in the buffer[prod_ptr]
                prod_ptr := (prod_ptr + 1) mod n;
                signal(CS_prod)
                signal(full)
            forever
        Parend;
    }
    consumer()
    {
        Parbegin
            repeat
                wait(full);
                wait(CS_cons);
                consume from the buffer[cons_ptr]
                cons_ptr := (cons_ptr + 1) mod n;
                signal(CS_cons)
                signal(empty)
            forever
        Parend;
    }
end
