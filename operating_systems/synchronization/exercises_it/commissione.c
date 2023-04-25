// Nella sala recruitment di una grande azienda informatica, durante una sessione di interview possono prendere posto al più N candidati,
// mentre i restanti candidati prenderanno parte alla sessione di interview successiva.
// Ogni candidato che ha trovato posto sarà chiamato al tavolo delle interview.
// L'addetto alle risorse umane attende fino che è disponibile un candidato. 
// Terminata l'intervista II candidato lascia il tavolo e la sala facendo posto al prossimo candidato.
// Le sessioni di interview terminano quando non ci sono più candidati in attesa. Proporre una soluzione e discutere eventuali problemi di deadlock.

var
    waiting : intenger = 0;
    is_interviewing : boolean = false;
    sem_CS : semaphore = 1;
    sem_tavolo : semaphore = 0;
    sem_addetto : semaphore = 0;
begin
    candidato()
    {
        Parbegin
            wait(sem_CS);
            if(waiting < N)
            then
                waiting := waiting + 1;
                signal(sem_CS);
                wait(sem_tavolo);
            else
                signal(sem_CS);
                leave();

            interview();
            
            wait(sem_CS);
            is_interviewing := false;
            signal(sem_CS);

            signal(sem_addetto);
            leave();
        Parend;
    }

    addetto()
    {
        Parbegin
            repeat
                wait(sem_CS);
                if(is_interviewing)
                then 
                    signal(sem_CS);
                    wait(sem_addetto);
                else
                    is_interviewing := true;
                    waiting := waiting - 1;
                    signal(sem_tavolo);
                    signal(sem_CS);
            until waiting = 0;
        Parend;
    }
end.
