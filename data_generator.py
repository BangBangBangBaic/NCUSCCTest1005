import numpy as np
import pandas as pd
dateint=np.random.randint(1,10000,size=100000)
datedouble=np.random.uniform(1.0,10000.0,size=100000)
pdint=pd.DataFrame(dateint,columns=['value'])
pdint.to_csv('data_int_100000.csv',index=False)
pddouble=pd.DataFrame(datedouble,columns=['value'])
pddouble.to_csv('data_double_100000.csv',index=False)
