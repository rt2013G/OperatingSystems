// Si vuole gestire la fila di un negozio dove aspettano N persone
// Una guardia si prende cura di far entrare un numero predefinito 
// di persone per volta all'interno del negozio, e all'uscita
// di queste ripete il suo ciclo
// Le persone aspettano il loro turno per l'entrata nel negozio,
// una volta entrati, aspettano che un commesso li serva,
// appena serviti, escono segnalando l'uscita
// Gli addetti, appena liberi, servono i clienti ed accedono ad una
// mensola di scarpe per trovare il paio che richiedono

var
    turn : semaphore := 0;
    in_the_store = semaphore := N;
    waiting : semaphore := 0;
    client : semaphore := 0;
    employee : semaphore := 0;
guardia()
{
    repeat
        wait(waiting);
        wait(in_the_store);
        signal(turn);
    forever
}

persona()
{
        signal(waiting);
        wait(turn);
        // in the store
        signal(client);
        wait(employee);
        buy_shoes();
        // outside the store
        signal(in_the_store)
}

addetto()
{
    repeat
        wait(shelf)
        wait(client);
        serve();
        signal(employee);
    forever
}
