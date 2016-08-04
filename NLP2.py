from time import clock

class NLP():
    def __init__(self):     #initial
        head = clock()
        self.wContainer = dict()
        self.pContainer = dict()
        self.readTrainingData()
        self.addDict()
        self.readPContainer()
        tail = clock()
        print "The time of initing is: ", tail-head
    
    def readTrainingData(self):     #read the word from training data
        print 
        print "Getting the traning data..."
        trainingData = open('trainingData/trainingData.txt', 'r')
        for line in trainingData.readlines():
            for termT in line.split():
                term = termT.split('/')
                if term[1] != 'w':
                    if term[0] in self.wContainer:
                        self.wContainer[term[0]] += 1
                    else:
                        self.wContainer[term[0]] = 1
        trainingData.close()
        print "Done!"
    
    def readPContainer(self):       #read the punctuation from the file
        print 
        print "Getting the punctuation..."
        punctuation = open("punctuation.txt", 'r')
        for line in punctuation.readlines():
            line = line.strip('\n')
            self.pContainer[line] = 1
        punctuation.close()
        print "Done!"
    
    def addDict(self):              #add dict into the divider
        print 
        print "Adding the dict..."
        dictData = open("trainingData/dictTrain.txt", "r")
        for line in dictData.readlines():
            term = line.split()
            if term[0] in self.wContainer:
                self.wContainer[term[0]] += 1
            else:
                self.wContainer[term[0]] = 1
        dictData.close()
        print 'Done!'
    
    def controller(self, tag):      #control the process of dividing
        print "Starting dividing",
        start = clock()
        if tag == '0':
            print "source/dev.txt..."
            source = open("source/dev.txt", "r")
            rOut = open("result/dev.txt", "w")
            print "The result of dividing is stored at 'result/dev.txt'"
        else:
            print "source/test.txt..."
            source = open("source/test.txt", "r")
            rOut = open("result/res.txt", "w")
            print "The result of dividing is stored at 'result/res.txt'"
        for line in source.readlines():
            part = ""
            counter = 0
            term = ""
            for i in line:
                term += str(i)
                counter += 1
                if counter == 2:
                    if term in self.pContainer:
                        self.divide(part, rOut)
                        part = ""
                        print >> rOut, term
                    else:
                        part += term
                    counter = 0
                    term = ""
            if len(part) > 0:
                self.divide(part, rOut)
                
        end = clock()
        print "Done!\nThe running time of dividing: ", end - start
        source.close()
        rOut.close()
    
    def divide(self, part, rOut):   #the main process of dividing
        termLen = len(part)/2
        
        eachTerm = range(termLen)   #get eachTerm
        for i in xrange(termLen):
            eachTerm[i] = str(part[i*2]) + str(part[i*2+1])
            
        maxLen = range(termLen)     #get the maxLen of each term
        tmpM = 1
        for i in xrange(termLen):
            term = ""
            for j in xrange(i, termLen):
                term += eachTerm[j]
                if term in self.wContainer:
                    tmpM = j - i + 1
            maxLen[i] = tmpM
            tmpM = 1

        indexContainer = set()      #get the each index
        for i in xrange(termLen):
            indexContainer.add(i)
        
        posContainer = set()       #to get the maxPostion
        while len(indexContainer) > 0:
            maxP = -1
            for i in indexContainer:
                if maxP == -1:
                    tag = 1                                     #check the I
                    for j in posContainer:                      #check the I
                        if i < j and (i + maxLen[i]) > j:       #check the I
                            tag = 0                             #check the I
                    if tag == 1:                                #check the I
                        maxP = i
                else:
                    if maxLen[i] > maxLen[maxP]:
                        tag = 1                                 #check the I
                        for j in posContainer:                  #check the I
                            if i < j and (i + maxLen[i]) > j:   #check the I
                                tag = 0                         #check the I
                        if tag == 1:                            #check the I
                            maxP = i
                    elif maxLen[i] == maxLen[maxP]:
                        if maxLen[i] == 2:
                            termI = eachTerm[i] + eachTerm[i+1]
                            termM = eachTerm[maxP] + eachTerm[maxP+1]
                            if self.wContainer[termI] > self.wContainer[termM]:
                                maxP = i
            if maxP == -1 or maxLen[maxP] < 2:
                break
            posContainer.add(maxP)
            for i in xrange(1, maxLen[maxP]):
                maxLen[maxP + i] = 0
                indexContainer.discard(maxP + i)
            indexContainer.discard(maxP)
            if maxP > 0 and 2 == maxLen[maxP - 1]:
                maxLen[maxP - 1] = 1
        
        for i in xrange(termLen):
            term = ""
            if maxLen[i] > 0:
                for j in xrange(maxLen[i]):
                    term += eachTerm[i+j]
                print >> rOut, term
                
    def outputWContainer(self):
        w = open("wContainer.txt", "w")
        for i in self.wContainer:
            print >> w, i + str(self.wContainer[i])
        w.close()
        print
    
    def showPContainer(self):
        print "The pContainer are:"
        for i in self.pContainer:
            print i

divider = NLP()
while True:
    tag = raw_input("Which source file(0 - dev.txt, 1 - test.txt) you wanna divide?\n")
    divider.controller(tag)
    print 
    end = raw_input("Continue(0-No, 1-yes)?\n")
    if (end == '0'):
        break
