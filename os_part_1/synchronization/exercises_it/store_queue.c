// Si vuole gestire la fila di un negozio dove aspettano N persone
// Una guardia si prende cura di far entrare un numero predefinito 
// di persone per volta all'interno del negozio, e all'uscita
// di queste ripete il suo ciclo
// Le persone aspettano il loro turno per l'entrata nel negozio
// fanno compere e poi escono, segnalando l'uscita

var
    turn : semaphore := 0;
    in_the_store = semaphore := N;
    waiting : semaphore := 0
guardia()
{
    Parbegin
        repeat
            wait(waiting);
            wait(group_size);
            signal(turn);
        forever
    Parend
}

persona()
{
    Parbegin
        signal(waiting);
        wait(turn);
        buy();
        signal(group_size);
    Parend
}
