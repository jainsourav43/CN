import pandas as pd 

def cost(data,theta0,theta1,theta2):
	sum=0
	for i in range(len(data)):
		if data[i][4] ==0 :
			sum=sum+(theta0+theta1*data[i][2]+theta2*data[i][10]-(data[i][1]-data[i][12]))*(theta0+theta1*data[i][2]+theta2*data[i][10]-(data[i][1]-data[i][12]))
		else:
			sum=sum+(theta0+theta1*data[i][2]+theta2*data[i][10]-((data[i][5]-data[i][4])+(data[i][1]-data[i][12])))*(theta0+theta1*data[i][2]+theta2*data[i][10]-((data[i][5]-data[i][4])+(data[i][1]-data[i][12])))
	sum=sum/(2*len(data))
	print(sum)

def cost2(data,theta0,theta1,theta2):
	sum=0
	for i in range(len(data)):
		if data[i][3]== True:
			sum=sum+(theta0+theta1*data[i][0]+theta2*data[i][1]-(data[i][4]-data[i][5]))*(theta0+theta1*data[i][0]+theta2*data[i][1]-(data[i][4]-data[i][5]))
	sum=sum/(2*len(data))
	print(sum)


def cost3(data,theta0,theta1,theta2):
	sum=0
	for i in range(len(data)):
		if data[i][3]== True:
			sum=sum+(theta0+theta1*data[i][0]+theta2*data[i][1]-data[i][4])*(theta0+theta1*data[i][0]+theta2*data[i][1]-data[i][4])
	sum=sum/(2*len(data))
	print(sum)

def gradient(data,theta0,theta1,theta2,num_iter,alpha):
	sum1=0
	sum2=0
	sum3=0
	m=len(data)
	for j in range(num_iter):
		#print(theta0,theta1,theta2)
		for i in range(len(data)):
			if data[i][4] == 0:
				sum1=sum1+(theta0+theta1*data[i][2]+theta2*data[i][10]-(data[i][1]-data[i][12]))
				sum2=sum2+(theta0+theta1*data[i][2]+theta2*data[i][10]-(data[i][1]-data[i][12]))*data[i][2]
				sum3=sum3+(theta0+theta1*data[i][2]+theta2*data[i][10]-(data[i][1]-data[i][12]))*data[i][10]
			else :
				sum1=sum1+(theta0+theta1*data[i][2]+theta2*data[i][10]-((data[i][5]-data[i][4])+(data[i][1]-data[i][12])))
				sum2=sum2+(theta0+theta1*data[i][2]+theta2*data[i][10]-((data[i][5]-data[i][4])+(data[i][1]-data[i][12])))*data[i][2]
				sum3=sum3+(theta0+theta1*data[i][2]+theta2*data[i][10]-((data[i][5]-data[i][4])+(data[i][1]-data[i][12])))*data[i][10]
		sum1=sum1/m
		sum2=sum2/m
		sum3=sum3/m
		theta0=theta0 - alpha*sum1
		theta1=theta1 - alpha*sum2
		theta2=theta2 - alpha*sum3
		#cost(data,theta0,theta1,theta2)
	result=[]
	result.append(theta0)
	result.append(theta1)
	result.append(theta2)
	return result

def gradient2(data,theta0,theta1,theta2,num_iter,alpha):
	sum1=0
	sum2=0
	sum3=0
	m=len(data)
	for j in range(num_iter):
		for i in range(len(data)):
			if data[i][3]== True:
				sum1=sum1+(theta0+theta1*data[i][0]+theta2*data[i][1]-(data[i][4]-data[i][5]))
				sum2=sum2+(theta0+theta1*data[i][0]+theta2*data[i][1]-(data[i][4]-data[i][5]))*data[i][0]
				sum3=sum3+(theta0+theta1*data[i][0]+theta2*data[i][1]-(data[i][4]-data[i][5]))*data[i][1]
		sum1=sum1/m
		sum2=sum2/m
		sum3=sum3/m
		theta0=theta0 - alpha*sum1
		theta1=theta1 - alpha*sum2
		theta2=theta2 - alpha*sum3
		#cost2(data,theta0,theta1,theta2)
	result=[]
	result.append(theta0)
	result.append(theta1)
	result.append(theta2)
	return result

def gradient3(data,theta0,theta1,theta2,num_iter,alpha):
	sum1=0
	sum2=0
	sum3=0
	m=len(data)
	for j in range(num_iter):
		for i in range(len(data)):
			if data[i][3] ==True:
				sum1=sum1+(theta0+theta1*data[i][0]+theta2*data[i][1]-data[i][4])
				sum2=sum2+(theta0+theta1*data[i][0]+theta2*data[i][1]-data[i][4])*data[i][0]
				sum3=sum3+(theta0+theta1*data[i][0]+theta2*data[i][1]-data[i][4])*data[i][1]
		sum1=sum1/m
		sum2=sum2/m
		sum3=sum3/m
		theta0=theta0 - alpha*sum1
		theta1=theta1 - alpha*sum2
		theta2=theta2 - alpha*sum3
		#cost3(data,theta0,theta1,theta2)
	result=[]
	result.append(theta0)
	result.append(theta1)
	result.append(theta2)
	return result







data =pd.read_csv('gcTrianingSet.csv')
theta0=0.0008
theta1=0.01
theta2=0.01
data1=data.loc[data['gcRun'] == False]
array =data.values
for i in range(len(array)):
	temp,array[i][2]=array[i][2].split("_")
for i in range(len(array)):
	array[i][2]=int(array[i][2])

#print(array)
numiter=500
alpha=0.000001
result1=gradient(array,theta0,theta1,theta2,numiter,alpha)
cost(array,theta0,theta1,theta2)
Theta0=0.1
Theta1=1
Theta2=0.5
alpha=0.0003
result2= gradient2(array,Theta0,Theta1,Theta2,numiter,alpha)
cost2(array,Theta0,Theta1,Theta2)
Theta00=0.1
Theta01=1
Theta02=0.5
alpha=0.0001
result3=gradient3(array,Theta00,Theta01,Theta02,numiter,alpha)
cost3(array,Theta00,Theta01,Theta02)
for i in range(len(result1)):
	print(result1[i])
for i in range(len(result2)):
	print(result2[i])
for i in range(len(result3)):
	print(result3[i])

data =pd.read_csv('gcPredictionFile.csv')
test=data.values

# print(test)
for i in range(len(test)):
	temp,test[i][2]=test[i][2].split("_")
for i in range(len(test)):
	test[i][2]=int(test[i][2])

for i in range(len(test)):
 	memoryused=result1[0]+result1[1]*test[i][2]+result1[2]*test[i][3]
 	threshold=result3[0]+result3[1]*test[i][0]+result3[2]*test[i][1]
 	print("memoryused= "  +str(memoryused)) 
 	print("Threshold= "+ str(threshold) )
 	test[i][4]=False
 	cleanmemory=0
 	if threshold < memoryused+test[i][0]:
 		cleanmemory = result2[0]+result2[1]*test[i][0]+result2[2]*test[i][1]
 		test[i][4]=True
 	finalmemory= test[i][0]+memoryused-cleanmemory
 	freememory=test[i][1]-memoryused+cleanmemory
 	if(i<len(test)-1):
 		test[i+1][0]=finalmemory
 		test[i+1][1]=freememory
 	test[i][2]="token_"+str(test[i][2])	
 	if test[i][4] ==True:
 		print(test[i])

df = pd.DataFrame(test,columns=['a', 'b', 'c', 'd', 'e'])
df1=df[['b','e']]

df1.to_csv("gcoutput.csv")

