# Presupunere: calatorii nu sunt neaparat unici, adica
# aparatul nu distinge daca clientul a mai mers sau nu
# in aceeasi zi cu tramvaiul si este numarat de mai multe
# ori. Aceasta presupunere va ajuta la calculul banilor
# de pe abonamente.

#a)

sansa_zi_lejera <- 0.2
sansa_zi_normala <- 0.5
sansa_zi_aglomerata <- 0.3

interval_zi_lejera <- 210:370
interval_zi_normala <- 371:650
interval_zi_aglomerata <- 651:983

medie_dorita <- 512
deviere_maxima <- 10

generare_calatori_zi <- function(tip_zi) {
  if(tip_zi=='lejera') {
    return(sample(interval_zi_lejera, 1))
  }
  else if(tip_zi == 'normala') {
    return(sample(interval_zi_normala, 1))
  }
  else return(sample(interval_zi_aglomerata, 1))
}

generare_calatori_luna <- function(nr_luna) {
  zile_in_luna <- c(31,28,31,30,31,30,31,31,30,31,30,31)
  nr_calatori <- rep(0, zile_in_luna[nr_luna])
  for(i in 1:zile_in_luna[nr_luna]) {
    tipuri_zile <- c('lejera','normala','aglomerata')
    prob <- c(sansa_zi_lejera, sansa_zi_normala, sansa_zi_aglomerata)
    
    tip_zi_aleasa <- sample(tipuri_zile, size=1, prob=prob)
    
    nr_calatori[i] <- generare_calatori_zi(tip_zi_aleasa)
  }
  
  return(nr_calatori)
}

luna_iulie <- 6

calatori_iunie <- generare_calatori_luna(luna_iulie)
medie_calatori_iunie <- mean(calatori_iunie)
while(medie_calatori_iunie < medie_dorita - deviere_maxima 
      || medie_calatori_iunie > medie_dorita + deviere_maxima
      ) {
  calatori_iunie <- generare_calatori_luna(luna_iulie)
  medie_calatori_iunie <- mean(calatori_iunie)
}

hist(calatori_iunie, 
     main = "Numar de calatori pe zi",
     xlab = "Numar calatori",
     ylab = "Frecventa aparitie",
     col = "blue",
     border = "black")

#print(medie_calatori_iunie)

######################################################
#b)

luni <- c('Ianuarie','Februarie','Martie','Aprilie','Mai',
          'Iunie','Iulie','August','Septembrie','Octombrie',
          'Noiembrie','Decembrie')

# Presupun 28 de zile in luna februarie >|
zile_in_luna <- c(31,28,31,30,31,30,31,31,30,31,30,31)

calatori_lunar <- rep(c(), 12)
calatori_lunar_medie <- rep(0, 12)
calatori_lunar_min <- rep(0, 12)
calatori_lunar_max <- rep(0, 12)
calatori_lunar_total <- rep(0, 12)
zile_lejere_proc <- rep(0, 12)
zile_normale_proc <- rep(0, 12)
zile_aglomerate_proc <- rep(0, 12)


for (i in 1:12) {
  calatori_lunar[[i]] <- generare_calatori_luna(i)
  medie_calatori <- mean(calatori_lunar[[i]])  # Corrected here
  while (medie_calatori < medie_dorita - deviere_maxima || medie_calatori > medie_dorita + deviere_maxima) {
    calatori_lunar[[i]] <- generare_calatori_luna(i)
    medie_calatori <- mean(calatori_lunar[[i]])
  }
  calatori_lunar_max[i] <- max(calatori_lunar[[i]])
  calatori_lunar_min[i] <- min(calatori_lunar[[i]])
  calatori_lunar_medie[i] <- round(mean(calatori_lunar[[i]]), 2)
  calatori_lunar_total[i] <- sum(calatori_lunar[[i]])
  
  zile_lejere <- 0
  zile_normale <- 0
  zile_aglomerate <- 0
  
  for(j in 1:zile_in_luna[i]) 
    if(calatori_lunar[[i]][[j]] %in% interval_zi_lejera)
      zile_lejere <- zile_lejere + 1
    else
      if(calatori_lunar[[i]][[j]] %in% interval_zi_normala)
        zile_normale <- zile_normale + 1
      else
        zile_aglomerate <- zile_aglomerate + 1
  
  zile_lejere_proc[i] <- round(zile_lejere / zile_in_luna[[i]] * 100, 2)
  zile_normale_proc[i] <- round(zile_normale / zile_in_luna[[i]] * 100, 2)
  zile_aglomerate_proc[i] <- round(zile_aglomerate / zile_in_luna[[i]] * 100, 2)
}


calatori_df <- data.frame(luna = luni,
                          calatori_total = calatori_lunar_total, 
                          calatori_medie = calatori_lunar_medie, 
                          calatori_minim = calatori_lunar_min, 
                          calatori_maxim = calatori_lunar_max,
                          zile_lejere_proc,
                          zile_normale_proc, 
                          zile_aglomerate_proc)

calatori_df

#############################################################################3
# c)

calatori_cu_abonament_lunar <- rep(0 ,12)
calatori_cu_bilet_lunar <- rep(0, 12) # fara abonament, dar cu bilet
calatori_fara_bilet_lunar <- rep(0, 12) # fara bilet sau abonament
calatori_fara_bilet_zi <- vector("list", 12)

medie_calatorii_abonament <- 2 * 5 * 4
# Presupunem 2 calatorii pe zi lucratoare

pret_abonament <- 70
pret_bilet <- 3

for (i in 1:12) {
  for (j in 1:zile_in_luna[i]) {  
    simulare_abonament <- sample(c(T,F), calatori_lunar[[i]][[j]],
                                 replace = T, prob = c(0.38, 1-0.38))
    
    calatori_cu_abonament <- sum(simulare_abonament == T)
    
    calatori_fara_abonament <- calatori_lunar[[i]][[j]] - calatori_cu_abonament
    # cei fara abonament pot sa aiba sau nu bilet
    
    simulare_bilet <- sample(c(T,F), calatori_fara_abonament, 
                             replace = T, prob = c(0.74, 1-0.74))
    
    calatori_cu_bilet <- sum(simulare_bilet == T)
    calatori_fara_bilet <- calatori_fara_abonament - calatori_cu_bilet
    
    calatori_fara_bilet_zi[[i]] <- c(calatori_fara_bilet_zi[[i]], calatori_fara_bilet)
    
    # actualizare date lunare
    calatori_cu_abonament_lunar[i] <- calatori_cu_abonament_lunar[i] + calatori_cu_abonament
    calatori_fara_bilet_lunar[i] <- calatori_fara_bilet_lunar[i] + calatori_fara_bilet
    calatori_cu_bilet_lunar[i] <- calatori_cu_bilet_lunar[i] + calatori_cu_bilet
  }
}

calatori_df$calatori_abonati <- calatori_cu_abonament_lunar
calatori_df$venit_abonamente <- round(calatori_df$calatori_abonati / medie_calatorii_abonament * pret_abonament)

calatori_df$calatori_bilet <- calatori_cu_bilet_lunar
calatori_df$venit_bilete <- calatori_df$calatori_bilet * pret_bilet

calatori_df$calatori_fraudulos <- calatori_fara_bilet_lunar
calatori_df$pierdere_frauda <- (-1) * calatori_df$calatori_fraudulos * pret_bilet

calatori_df$venit_net <- calatori_df$venit_abonamente + calatori_df$venit_bilete

calatori_df


par(mar = c(6, 6, 4, 2) + 0.1) 

barplot(calatori_df$venit_net,
        main = "Venit net pe luna",
        names.arg = calatori_df$luna,
        xlab = "Luna",
        ylab = "Venit net",
        col = "blue",
        ylim = c(0, max(calatori_df$venit_net)), 
        las = 2,
        yaxt = "n")

axis(2, at = seq(0, max(calatori_df$venit_net), by = 1000), las = 2)


################################################
# d)

# Observatie: controlorul din enunt este cam scump: 
# STB iese pe minus in orice zi lejera sau aglomerata
# iar in zilele normale pe plus cu doar 36 lei

# (pentru fiecare din cele 2 verificări zilnice există
# un cost asociat controlorului de 214 lei) inseamna
# ca fiecare verificare costa 214 lei ?


# Presupunem ca se extrage din toata populatia, chiar
# daca nu se afla toata intr-un singur tramvai, pentru ca
# nu stim distributia fiecarei curse in parte.

calatori_fara_bilet_zi_tmp <- calatori_fara_bilet_zi
calatori_lunar_tmp <- calatori_lunar
# redefinim variabilele pentru ca le vom modifica (vom scoate
# din populatie calatorii deja controlati)

valoare_amenda <- 50
cost_controlor <- 214
sansa_raportare_amenda <- 1 - 0.3

venit_amenzi <- vector("list", 12)

for(i in 1:12)
  for(j in 1:zile_in_luna[i])
    venit_amenzi[[i]] <- c(venit_amenzi[[i]], 0)

venit_amenzi_net <- venit_amenzi

control <- function(manarie_controlor = F) {
  for(i in 1:12) {
    for(j in 1:zile_in_luna[i]) {
      populatie <- c(rep(F, calatori_fara_bilet_zi_tmp[[i]][j]), rep(T, calatori_lunar_tmp[[i]][j] - calatori_fara_bilet_zi_tmp[[i]][j]))
      
      if(calatori_lunar[[i]][j] %in% interval_zi_lejera) {
        persoane_controlate_max <- sample(2:11, 1)
        
        rezultat_control <- sample(populatie, persoane_controlate_max, replace = FALSE)
        
        # trunchiere la al treilea control fara bilet
        pozitii_true <- which(rezultat_control == F)
        if(length(pozitii_true) >= 3)
          rezultat_control <- rezultat_control[1:pozitii_true[3]]
        
        # cate persoane s-au controlat de fapt; pentru ca
        # se opreste la 3 fara bilet
        persoane_controlate <- length(rezultat_control)
        
        # amenzi neraportate se transforma in calatori legitimi
        if(manarie_controlor == T)
          for(k in 1:persoane_controlate)
            if(rezultat_control[k] == F)
              rezultat_control[k] <- sample(c(T, F), 1, prob=c(1 - sansa_raportare_amenda, sansa_raportare_amenda))
        # sansa sa ramana fals (adica nu are bilet) este 0.7
        # sansa sa devina adevarat (legitim) este 0.3
        # doar daca este specificat parametrul manarie_controlor
        
        numar_amenzi_control_curent <- sum(rezultat_control == F)
        venit_amenzi[[i]][j] <<- venit_amenzi[[i]][j] + numar_amenzi_control_curent * valoare_amenda
        
        # stergerea persoanelor controlate din populatie
        
        calatori_fara_bilet_zi_tmp[[i]][j] <<- calatori_fara_bilet_zi_tmp[[i]][j] - numar_amenzi_control_curent
        calatori_lunar_tmp[[i]][j] <<- calatori_lunar_tmp[[i]][j] - persoane_controlate
      }
      else if(calatori_lunar[[i]][j] %in% interval_zi_normala) {
        persoane_controlate_max <- sample(1:calatori_lunar_tmp[[i]][j], 1)
        
        rezultat_control <- sample(populatie, persoane_controlate_max, replace = FALSE)
        
        # trunchiere la al 5-lea control fara bilet
        pozitii_true <- which(rezultat_control == F)
        if(length(pozitii_true) >= 5)
          rezultat_control <- rezultat_control[1:pozitii_true[5]]
        
        persoane_controlate <- length(rezultat_control)
        
        # amenzi neraportate se transforma in calatori legitimi
        if(manarie_controlor == T)
          for(k in 1:persoane_controlate)
            if(rezultat_control[k] == F)
              rezultat_control[k] <- sample(c(T, F), 1, prob=c(1 - sansa_raportare_amenda, sansa_raportare_amenda))
        
        numar_amenzi_control_curent <- sum(rezultat_control == F)
        venit_amenzi[[i]][j] <<- venit_amenzi[[i]][j] + numar_amenzi_control_curent * valoare_amenda
        
        # stergerea persoanelor controlate din populatie
        
        calatori_fara_bilet_zi_tmp[[i]][j] <<- calatori_fara_bilet_zi_tmp[[i]][j] - numar_amenzi_control_curent
        calatori_lunar_tmp[[i]][j] <<- calatori_lunar_tmp[[i]][j] - persoane_controlate
      }
      else  {
        persoane_controlate_max <- sample(3:5, 1)
        
        rezultat_control <- sample(populatie, persoane_controlate_max, replace = FALSE)
        
        # trunchiere la al 5-lea control fara bilet
        pozitii_true <- which(rezultat_control == F)
        if(length(pozitii_true) >= 1)
          rezultat_control <- rezultat_control[1:pozitii_true[1]]
        
        persoane_controlate <- length(rezultat_control)
        
        # amenzi neraportate se transforma in calatori legitimi
        if(manarie_controlor == T)
          for(k in 1:persoane_controlate)
            if(rezultat_control[k] == F)
              rezultat_control[k] <- sample(c(T, F), 1, prob=c(1 - sansa_raportare_amenda, sansa_raportare_amenda))
        
        numar_amenzi_control_curent <- sum(rezultat_control == F)
        venit_amenzi[[i]][j] <<- venit_amenzi[[i]][j] + numar_amenzi_control_curent * valoare_amenda
        
        
        # stergerea persoanelor controlate din populatie
        
        calatori_fara_bilet_zi_tmp[[i]][j] <<- calatori_fara_bilet_zi_tmp[[i]][j] - numar_amenzi_control_curent
        calatori_lunar_tmp[[i]][j] <<- calatori_lunar_tmp[[i]][j] - persoane_controlate
      }
      
      
      venit_amenzi_net[[i]][j] <<- venit_amenzi_net[[i]][j] + numar_amenzi_control_curent * valoare_amenda - cost_controlor
    }
  }
}

control() 
control() # doua controale pe zi

venit_amenzi_d <- venit_amenzi
venit_amenzi_d

venit_amenzi_net_d <- venit_amenzi_net
venit_amenzi_net_d

venit_amenzi_total_luna_d <- sapply(venit_amenzi_d, sum)
#venit_amenzi_total_luna_d

venit_amenzi_net_total_luna_d <- sapply(venit_amenzi_net_d, sum)
#venit_amenzi_net_total_luna

# Sunt sanse extrem de mici ca totalul pe luna sa fie pozitiv.
# In plus, suma de plata este mare relativ la venitul din bilete.

venit_amenzi_total_an_d <- sum(venit_amenzi_total_luna_d)
venit_amenzi_net_total_an_d <- sum(venit_amenzi_net_total_luna_d)

# eventual, comparat cu:
#print(calatori_df$venit_net)

numara_pozitive <- function(x) {
  sum(x > 0)
} 

numar_zile_profit_d <- sapply(venit_amenzi_net_d, numara_pozitive)
numar_zile_profit_d

medie_zile_profit_d <- mean(numar_zile_profit_d)
medie_zile_profit_d

##################################################
#e)


calatori_fara_bilet_zi_tmp <- calatori_fara_bilet_zi
calatori_lunar_tmp <- calatori_lunar
# redefinim variabilele pentru ca le vom modifica, la fel 
# ca la punctul d)

venit_amenzi <- vector("list", 12)

for(i in 1:12)
  for(j in 1:zile_in_luna[i])
    venit_amenzi[[i]] <- c(venit_amenzi[[i]], 0)

venit_amenzi_net <- venit_amenzi


control(manarie_controlor = TRUE)
control(manarie_controlor = TRUE)
control(manarie_controlor = TRUE)

venit_amenzi_e <- venit_amenzi
venit_amenzi_net_e <- venit_amenzi_net

venit_amenzi_total_luna_e <- sapply(venit_amenzi_e, sum)
#venit_amenzi_total_luna_e

venit_amenzi_net_total_luna_e <- sapply(venit_amenzi_net_e, sum)
#venit_amenzi_net_total_luna_e

# Sunt sanse extrem de mici ca totalul pe luna sa fie pozitiv.
# In plus, suma de plata este mare relativ la venitul din bilete.

venit_amenzi_total_an_e <- sum(venit_amenzi_total_luna_e)
venit_amenzi_net_total_an_e <- sum(venit_amenzi_net_total_luna_e)

numara_pozitive <- function(x) {
  sum(x > 0)
} 

numar_zile_profit_e <- sapply(venit_amenzi_net_e, numara_pozitive)
numar_zile_profit_e

medie_zile_profit_e <- mean(numar_zile_profit_e)
medie_zile_profit_e

