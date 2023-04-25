// In una pizzeria, due pizzaioli sfornano continuamente pizze
// che pongono di volta in volta in uno di N piatti disponibili su un bancone
// I piatti sono prelevati da M (M < N) camierieri che provvedono a servirli
// Ciascun camieriere può portare due pizze per volta se disponibili
// Ma in caso di affollamento i camierieri preleveranno 3 pizze per volte

var
    pizze_disponibili : integer := 0; // numero di piatti utilizzati sul bancone
    pizza_da_prendere : integer := 0; // numero di piatti da prelevare dal bancone
    cont_clienti : integer := 0;
    affollato : boolean := false;
    sem_CS : semaphore := 1;
    sem_CS_clientela : semaphore := 1;
    sem_pizze : semaphore := 0;
    sem_bancone : semaphore := 0;
begin
    pizzaiolo()
    {
        Parbegin
            repeat
                wait(sem_CS);
                if(pizze_disponibili >= N) // Se il bancone è pieno
                then
                    signal(sem_CS);
                    wait(sem_bancone); // Attendo che un camieriere prenda una pizza
                else 
                    signal(sem_CS);

                wait(sem_CS);
                sforna_pizza();
                pizze_disponibili := pizze_disponibili + 1;
                signal(sem_CS);

                signal(sem_pizze); // Segnalo a un camieriere che è disponibile una pizza
            forever
        Parend;
    }

    camieriere()
    {
        Parbegin
            repeat
                wait(sem_CS);
                if(affollato())
                then pizze_da_prendere := 3;
                else pizze_da_prendere := 2;
                signal(sem_CS);

                wait(sem_CS);
                if(pizze_disponibili >= pizze_da_prendere)
                then
                    prendi_pizze(pizze_da_prendere);
                    pizze_disponibili := pizze_disponibili - pizze_da_prendere;
                signal(sem_bancone);
                signal(sem_CS);
                else
                    signal(sem_CS);
                    wait(sem_pizze); // Attendo che un pizzaiolo prepari una pizza
            forever
        Parend;
    }

    cliente()
    {
        Parbegin
            repeat
                attendi();
                cont_clienti := cont_clienti + 1;

                wait(sem_CS_clientela);
                if(cont_clienti >= 10)
                then
                    affollato := true;
                else 
                    affollato := false;
                signal(sem_CS_clientela);
                end

            forever
        Parend;
    }
    
end.
