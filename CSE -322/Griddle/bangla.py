def segment(s):
    l=[]
    i=0
    j=len(s)
    mark=2509
    between=[2494,2520]
    while(i<j):
        r=s[i]
        i+=1
        while(i<j and ord(s[i])>=between[0] and ord(s[i])<between[1]):
            if(ord(s[i])==mark):
                r+=s[i]
                i+=1
            r+=s[i]
            i+=1
        l.append(r)
    return l


if __name__ == "__main__":
    s="মুগ্ধতা"
    s="লশুনপলাণ্ডগুঞ্জনকুম্ভীশ্রাপথন্নসুতকান্নাভোজ্যান্যমধুমাংসমূত্ররেতোহমেধ্যাভক্ষভক্ষণেগায়ত্র‍্যাষ্টসহ"
    print(s)
    l=segment(s)
    print(l)