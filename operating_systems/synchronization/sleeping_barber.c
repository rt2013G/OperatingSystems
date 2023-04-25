// Pseudocode for the sleeping barber problem.
// 1 barber, 1 armchair, N chairs
// If there are no customers, barber sleeps
// When a customer arrives, the customer wakes the barber
// If a custom arrives while the barber is busy, the customer waits in a chair
// If all chairs are occupied, the customer leaves

var
    sem_CS: semaphore := 1;
    customerReady, armchair : semaphore := 0;
    waiting : integer := 0;
    chairs : const := N;
begin
    barber()
    {
        Parbegin
            repeat
                wait(customerReady);
                wait(sem_CS);
                waiting := waiting - 1;
                signal(armchair);
                signal(sem_CS);
                cut_hair();
            forever
        Parend;
    }
    customer()
    {
        Parbegin
            repeat
                wait(sem_CS);
                if waiting < chairs 
                then
                    waiting := waiting + 1;
                    signal(customerReady);
                    signal(sem_CS);
                    wait(armchair);
                    get_haircut();
                else
                    signal(sem_CS);
                leave();
            forever
        Parend;
    }
end
