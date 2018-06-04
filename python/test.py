import ctypes  
from Tkinter import *
from tkMessageBox import *

def beginTest():
  CodeMethod = entry1.get()
  MemSize = entry2.get()
  ExperTimes = entry3.get()
  MBU = entry4.get()
  SEU = entry5.get()
  if (CodeMethod == '' or MemSize == '' or ExperTimes == '' or MBU == '' or SEU == ''):
	print 'please input the parameters'
	showerror("Answer", "Sorry, please input the parameters")
	return
  else :
  	print CodeMethod + ' ' + MemSize + ' ' + ExperTimes + ' ' + MBU + ' ' + SEU
	if CodeMethod == 'BCH_XOR':					#the coding Method 1 means BCH_XOR
		int_CodeMethod = 1
	elif CodeMethod == 'EDAC':
		int_CodeMethod = 0						#or EDAC coding Method
	else :
		int_CodeMethod = -1						#other undefined coding Method 
	int_MemSize = int(MemSize) * 8 * 1024 * 1024		#the bit number of memory
  	int_ExperTimes = int(ExperTimes)			#the reps num 
  	int_MBU = int(MBU)							#the probability of MBU
  	int_SEU = int(SEU)							#the probability of SEU
	so.test(int_CodeMethod, int_MemSize, int_ExperTimes, int_MBU, int_SEU)	#exec the C function
def endTest():
	print 'the test end!\n'
	return 

root=Tk()
root.title('MonteTest')
root.geometry('520x300')
so = ctypes.CDLL("./libmonte.so") 


label1=Label(root,text='CodeMethod',font=('Courier New', '13'),anchor='e')
label1.place(relx=0.05,rely=0.08,
             relwidth=0.2,relheight=0.05)
e = StringVar()
entry1=Entry(root, textvariable = e)
e.set('BCH_XOR')								#the coding Method,default method is BCH_XOR
entry1.place(relx=0.3,rely=0.05,
             relwidth=0.5,relheight=0.08)

label2=Label(root,text='MemSize',font=('Courier New', '13'),anchor='e')
label2.place(relx=0.05,rely=0.16,
             relwidth=0.2,relheight=0.05)
e = StringVar()
entry2=Entry(root, textvariable = e)
e.set('2')										#the memsize(MB), it need to convert when initation, the bit num is memsize * 8 * 1024 * 1024
entry2.place(relx=0.3,rely=0.13,
             relwidth=0.5,relheight=0.08)

label3=Label(root,text='ExperTimes',font=('Courier New', '13'),anchor='e')
label3.place(relx=0.05,rely=0.23,
             relwidth=0.2,relheight=0.05)
e = StringVar()
entry3=Entry(root, textvariable = e)
e.set('1')										#the Experiment times
entry3.place(relx=0.3,rely=0.21,
             relwidth=0.5,relheight=0.08)

label4=Label(root,text='MBU',font=('Courier New', '13'),anchor='e')
label4.place(relx=0.05,rely=0.31,
             relwidth=0.2,relheight=0.05)
e = StringVar()
entry4=Entry(root, textvariable = e)
e.set('2500')									#the probability of MBU,2500 means, An MBU event occurs every 2500 events
entry4.place(relx=0.3,rely=0.29,
             relwidth=0.5,relheight=0.08)

label5=Label(root,text='SEU',font=('Courier New', '13'),anchor='e')
label5.place(relx=0.05,rely=0.39,
             relwidth=0.2,relheight=0.05)
e = StringVar()
entry5=Entry(root, textvariable = e)
e.set('20000000')								#the probability of SEU,20000000 means, the SEU probility is 1/20000000
entry5.place(relx=0.3,rely=0.37,
             relwidth=0.5,relheight=0.08)

button1=Button(root, command=beginTest, text='BeginTest', font=('Courier New', '13','bold'))
button1.place(relx=0.30, rely=0.65,				#the beginTest button 
             relwidth=0.2, relheight=0.2)

button2=Button(root, command=endTest, text='EndTest', font=('Courier New', '13','bold'))
button2.place(relx=0.60, rely=0.65,				#the endTest button 
             relwidth=0.2, relheight=0.2)

root.mainloop()
