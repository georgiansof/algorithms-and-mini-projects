import numpy as np
import matplotlib.pyplot as plt
import prettytable as tbl
from scipy.optimize import curve_fit

Rv = 200000
Ra = 9

def actiunea_camp_magnetic():
    R = np.array([5,4,3,2])
    I = np.array([1.24,1.6,2.14,3.3])
    U = 178.1
    B = 0.716 * (4 * np.pi * 1e-7) * 154 * I / 0.2
    print(f'B = ', B)
    e_m = 2 * 178.1 / ((B**2) * (R**2))
    print(f'e/m = ', e_m)
    
#actiunea_camp_magnetic()

def objective_aval(R, a):
    return R / a

def objective_amonte(R, a):
    return a / R


def amonte_aval():
    #aval
    print('Aval')
    R_cobai = np.array([i * 100 for i in range(4,25)])
    U = np.array([3.5,3.6,3.7,3.8,3.8,3.8,3.9,8.9,9,9,9.1,9.1,9,9.1,9.1,9.2,9.2,9.2,9.2,9.2,9.3])
    I = np.array([0.0081, 0.0068, 0.0058, 0.005, 0.0045, 0.004, 0.0036, 0.0076, 0.0071, 0.006, 0.0061, 0.0058, 0.0054, 0.0051, 0.0048, 0.0046, 0.0044, 0.0042, 0.004, 0.0038, 0.0037])

    R1 = U / I
    R = U / (I - U/Rv)        
    eps = np.abs((R1 - R)/R)

    R = np.around(R, 4)
    I = np.around(I, 4)
    U = np.around(U, 4)
    R1 = np.around(R1, 4)
    eps = np.around(eps, 4)

    tabel = tbl.PrettyTable()
    tabel.add_column("R cobai", R_cobai)
    tabel.add_column("U", U)
    tabel.add_column("I", I)
    tabel.add_column("R1", R1)
    tabel.add_column("R", R)
    tabel.add_column("Err", eps)
    print(tabel)

    eps_av = eps
    R_av = R

    #amonte
    print('Amonte')
    R_cobai = np.array([i * 100 for i in range(4,25)])
    U = np.array([3.4,3.5,3.6,3.7,3.7,8,8.1,8.2,8.2,8.3,8.3,8.4,8.4,8.4,8.4,8.4,8.5,8.5,8.5,8.6,8.6])
    I = np.array([0.0076,0.0064,0.0055,0.0048,0.0043,0.0082,0.0074,0.0068,0.0064,0.0059,0.0055,0.0052,0.0049,0.0046,0.0044,0.0042,0.004,0.0038,0.0036,0.0035,0.0034])

    R1 = U / I
    R = (U - Ra * I ) / I       
    eps = np.abs((R1 - R)/R)

    R = np.around(R, 4)
    I = np.around(I, 4)
    U = np.around(U, 4)
    R1 = np.around(R1, 4)
    eps = np.around(eps, 4)

    tabel = tbl.PrettyTable()
    tabel.add_column("R cobai", R_cobai)
    tabel.add_column("U", U)
    tabel.add_column("I", I)
    tabel.add_column("R1", R1)
    tabel.add_column("R", R)
    tabel.add_column("Err", eps)
    print(tabel)
    
    eps_am = eps
    R_am = R

    popt_av = curve_fit(objective_aval, R_av, eps_av)
    popt_am = curve_fit(objective_amonte, R_am, eps_am)

    param_av = popt_av[0]
    param_am = popt_am[0]

    x_regresie_am = np.linspace(R_am[0],R_am[20], 250)
    x_regresie_av = np.linspace(R_av[0],R_av[20], 250)

    y_regresie_am = objective_amonte(x_regresie_am, param_am)
    y_regresie_av = objective_aval(x_regresie_av, param_av)


    fig = plt.figure()
    ax = plt.axes()
    ax.plot(R_am, eps_am * 100, linestyle = 'None', marker = 'o', markersize = 7, color = 'b', label = 'ε am = f(R am)')
    ax.plot(R_av, eps_av * 100, linestyle = 'None', marker = 'o', markersize = 7, color = 'r', label = 'ε av = g(R av)')
    ax.plot(x_regresie_av, y_regresie_av * 100, linestyle = '--', color = 'r', lw = 3, label = 'Regresie aval')
    ax.plot(x_regresie_am, y_regresie_am * 100, linestyle = '--', color = 'b', lw = 3, label = 'Regresie amonte')
    ax.plot()

    
    ax.axhline(y=0, color = 'k', lw = 5)
    ax.axvline(x=350, color = 'k', lw = 5)
    ax.grid(which='major', axis = 'both', color='#DDDDDD', linestyle='-')
    ax.grid(which='minor', axis = 'both', color='#EEEEEE', linestyle=':')
    ax.minorticks_on()
    ax.legend(loc = 'best')
    plt.xlabel("R (Ω)", fontsize = 18)
    plt.ylabel("ε (%)", fontsize = 18)
    ax.xaxis.labelpad = 20
    ax.yaxis.labelpad = 20
    ax.xaxis.set_ticks(np.arange(250,2600,250))
    ax.yaxis.set_ticks(np.arange(0,2.25, 0.25))
    ax.set_xlim(xmin = 250)
    fig.tight_layout()
    plt.show()

#amonte_aval()

def liniar_func(x,a):
    return a*x

def masurarea_rezistentelor_mici():
    # Al
    U_Al = np.array([0,0.19,0.37,0.51,0.66,0.84,1,1.26,1.44,1.6,1.73,1.98,2.17])
    I_Al = np.array([0,0.1,0.2,0.31,0.4,0.5,0.6,0.73,0.8,0.91,1,1.1,1.2])
    U_Al /= 1e5
    
    #Cu
    U_Cu = np.array([0,0.17,0.27,0.34,0.47,0.51,0.68,0.81,0.93,1.06,1.14,1.31,1.43])
    I_Cu = np.array([0,0.1,0.2,0.3,0.41,0.5,0.61,0.71,0.8,0.9,1,1.1,1.2])
    U_Cu /= 1e5

    popt_Al = curve_fit(liniar_func, U_Al, I_Al)
    popt_Cu = curve_fit(liniar_func, U_Cu, I_Cu)

    param_Al = popt_Al[0]
    param_Cu = popt_Cu[0]

    x_regresie_Al = np.linspace(U_Al[0],U_Al[12], 250)
    x_regresie_Cu = np.linspace(U_Cu[0],U_Cu[12], 250)

    y_regresie_Al = liniar_func(x_regresie_Al, param_Al)
    y_regresie_Cu = liniar_func(x_regresie_Cu, param_Cu)

    fig = plt.figure()
    ax = plt.axes()
    ax.plot(U_Al, I_Al, linestyle = 'None', marker = 'o', markersize = 7, color = 'b', label = 'I = U/R (Al)')
    ax.plot(U_Cu, I_Al, linestyle = 'None', marker = 'o', markersize = 7, color = 'r', label = 'I = U/R (Cu)')

    ax.plot(x_regresie_Al, y_regresie_Al, linestyle = '-', lw = 3, color = 'b', label = 'Regresie Al')
    ax.plot(x_regresie_Cu, y_regresie_Cu, linestyle = '--', lw = 3, color = 'r', label = 'Regresie Cu')
    
    ax.axhline(y=0, color = 'k', lw = 5)
    ax.axvline(x=0, color = 'k', lw = 5)
    ax.grid(which='major', axis = 'both', color='#DDDDDD', linestyle='-')
    ax.grid(which='minor', axis = 'both', color='#EEEEEE', linestyle=':')
    ax.minorticks_on()
    ax.legend(loc = 'best')
    plt.xlabel("U (V)", fontsize = 18)
    plt.ylabel("I (A)", fontsize = 18)
    ax.xaxis.labelpad = 20
    ax.yaxis.labelpad = 20
    ax.xaxis.set_ticks(np.arange(0,2.25 * 1e-5,0.2 * 1e-5))
    ax.yaxis.set_ticks(np.arange(0,1.3, 0.2))
    fig.tight_layout()
    plt.show()
    print('1/R Al ', param_Al, 'Ω^(-1)')
    print('1/R Cu ', param_Cu, 'Ω^(-1)')
    R_Al = 1/param_Al
    R_Cu = 1/param_Cu
    S = 5.06 * 1e-4
    l = 0.315
    rho_al = R_Al * S / l
    rho_cu = R_Cu * S / l
    rho_cu_real = "1.68 * 10^(-8)"
    rho_al_real = "2.82 * 10^(-8)"
    tabel = tbl.PrettyTable()
    tabel.add_column("Material",["Cu","Al"])
    tabel.add_column("R(Ω)", [R_Cu,R_Al])
    tabel.add_column("ρ_exp(Ω*m)", [rho_cu,rho_al])
    tabel.add_column("ρ_real(Ω*m)", [rho_cu_real,rho_al_real ])
    print(tabel)


masurarea_rezistentelor_mici()

R01 = 500
R02 = 4
T0 = 18 + 271.15
B = np.zeros(14)

def func_metal(T,alpha):
    return R02 * (1 + alpha * (T-T0))

def func_semicond(T,a,b):
    return a * np.e ** (b/T)
    #return R01 - alpha ** (T0-T)


def variatia_rezistentei_cu_temperatura():
    tC = np.array([18,20,30,35,40,50,60,70,80,90,100,110,120,125])
    R1 = np.array([500,410,330,270,210,155,105,81,53,41,31,25,20,16.6])
    R2 = np.array([4,4.2,4.25,4.15,4.2,4.2,4.1,4.1,4.1,4.4,4.3,4.45,4.4,4.5])
    tK = tC + 271.15    

    popt_Met = curve_fit(func_metal, tK, R2)
    popt_Semicond = curve_fit(func_semicond, tK, R1)

    param_Met = popt_Met[0]
    param_Semicond = popt_Semicond[0]

    x_regresie_Met = np.linspace(tK[0],tK[13], 250)
    x_regresie_Semicond = x_regresie_Met

    y_regresie_Met = func_metal(x_regresie_Met, param_Met)
    y_regresie_Semicond = func_semicond(x_regresie_Semicond, param_Semicond[0], param_Semicond[1])

    fig = plt.figure()
    ax = plt.axes()
    #ax.plot(tK, R1, linestyle = 'None', marker = 'o', markersize = 5, color = 'b', label = 'I = U/R (Al)')
    ax.plot(tK, R2, linestyle = 'None', marker = 'o', markersize = 5, color = 'r', label = 'R = R0(1+αT)')

    ax.plot(x_regresie_Met, y_regresie_Met, linestyle = '-', lw = 2, color = 'r', label = 'Regresie Metal')
    #ax.plot(x_regresie_Semicond, y_regresie_Semicond, linestyle = '--', lw = 2, color = 'b', label = 'Regresie Semiconductor')
    
    ax.axhline(y=3.9, color = 'k', lw = 3)
    ax.axvline(x=280, color = 'k', lw = 3)
    ax.grid(which='major', axis = 'both', color='#DDDDDD', linestyle='-')
    ax.grid(which='minor', axis = 'both', color='#EEEEEE', linestyle=':')
    ax.minorticks_on()
    ax.legend(loc = 'best')
    plt.xlabel("T (K)", fontsize = 18)
    plt.ylabel("R (Ω)", fontsize = 18)
    ax.xaxis.labelpad = 20
    ax.yaxis.labelpad = 20
    ax.set_xlim(270)
    ax.set_ylim(3.8)
    #ax.xaxis.set_ticks(np.arange(0,2.25 * 1e-5,0.2 * 1e-5))
    #ax.yaxis.set_ticks(np.arange(0,1.3, 0.2))
    fig.tight_layout()
    #plt.show()

    fig = plt.figure()
    ax = plt.axes()
    ax.plot(tK, R1, linestyle = 'None', marker = 'o', markersize = 5, color = 'b', label = 'R = A*e^(B/T)')

    ax.plot(x_regresie_Semicond, y_regresie_Semicond, linestyle = '--', lw = 2, color = 'b', label = 'Regresie Semiconductor')
    
    ax.axhline(y=0, color = 'k', lw = 3)
    ax.axvline(x=280, color = 'k', lw = 3)
    ax.grid(which='major', axis = 'both', color='#DDDDDD', linestyle='-')
    ax.grid(which='minor', axis = 'both', color='#EEEEEE', linestyle=':')
    ax.minorticks_on()
    ax.legend(loc = 'best')
    plt.xlabel("T (K)", fontsize = 18)
    plt.ylabel("R (Ω)", fontsize = 18)
    ax.xaxis.labelpad = 20
    ax.yaxis.labelpad = 20
    ax.set_xlim(270)
    #ax.xaxis.set_ticks(np.arange(0,2.25 * 1e-5,0.2 * 1e-5))
    #ax.yaxis.set_ticks(np.arange(0,1.3, 0.2))
    fig.tight_layout()
    plt.show()

    RS = R1
    RM = R2

    B[1:] = 2.303 * (np.log10(RS[1:]) - np.log10(RS[0])) / (1/tK[1:] - 1/tK[0])
    B[0] = 0
    
    B_ = np.average(B[2:])

    tabel = tbl.PrettyTable()
    tabel.add_column("Nr. crt.", range(14))
    tabel.add_column("t(°C)", tC)
    tabel.add_column("t(K)", tK)
    tabel.add_column("R_M (Ω)", RM)
    tabel.add_column("R_S (Ω)", RS)
    tabel.add_column("B", B)
    

    print(tabel)
    print("B mediu (Nr. crt. 2->13): ", B_)
    print("α metal: ", param_Met)


#variatia_rezistentei_cu_temperatura()