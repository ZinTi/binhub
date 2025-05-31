#!/usr/bin/python
print("KiB/MiB/GiB_Binary, KB/MB/GB_Decimal")
while True:
    s1=input("select 1.GiB/GB->GB/GiB, 0.exit: ")
    if s1=='0':
        break
    elif s1=='1':
        v1=eval(input("value: "))
        print("{}GiB={}MiB={}KiB={}Byte={}KB={}MB={}GB".format(v1,v1*1024,v1*1024**2,v1*1024**3,v1*1024**3/1000,v1*1024**3/(1000**2),v1*1024**3/(1000**3)))
        print("{}GB={}MB={}KB={}Byte={}KiB={}MiB={}GiB".format(v1,v1*1000,v1*1000**2,v1*1000**3,v1*1000**3/1024,v1*1000**3/(1024**2),v1*1000**3/(1024**3)))
    else:
        print("error!")
print("Goodbye!")
