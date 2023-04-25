// CiccioGamer89 vuole realizzare il chest opening più grande d'Italia
// Per fare ciò, intende aprire almeno N bauli sul famoso videogioco Clash Royale
// In cambio di un certo numero di gemme, il gioco fornisce un certo numero M di bauli
// I bauli vengono aperto uno per volta. Quando tutti gli M bauli sono stati
// aperti, Ciccio acquista ulteriori gemme da convertire in ulteriori bauli e continua con l'apertura.
// Il programma si arresta quando Ciccio ha aperto almeno N bauli.

var
    bauli_aperti : integer := 0;
    bauli_disponibili : integer := 0;
    sem_CS : semaphore := 1;
    sem_bauli : semaphore := 0;
    sem_ciccio : semaphore := 0;
begin
    ciccio()
    {
        Parbegin
            repeat
                wait(sem_CS);
                if(bauli_disponibili <= 0)
                then
                    signal(sem_CS);
                    signal(sem_bauli);
                    wait(sem_ciccio);
                else
                    signal(sem_CS);
                
                wait(sem_CS);
                bauli_aperti := bauli_aperti + bauli_disponibili;
                bauli_disponibili := 0;
                signal(sem_CS);

            until (bauli_aperti >= N)
        Parend;
    }
    bauli()
    {
        Parbegin
            repeat
                wait(sem_CS);
                if(bauli_disponibili <= 0)
                then
                    bauli_disponibili := bauli_disponibili + M;
                    signal(sem_CS);
                    signal(sem_ciccio);
                    wait(sem_bauli);
                else
                    signal(sem_CS);
                    wait(sem_bauli);
            until (bauli_aperti >= N)
        Parend;
    }
end.
