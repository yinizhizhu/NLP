from time import clock

start = clock()

class divider():
    def __init__(self):
        self.words = dict()
        self.symbol = dict()
        self.result = open("1130310119.txt", "w")
    
    def readData(self):
        data = open('trainingData.txt', 'r')
        for line in data.readlines():
            for wordWithTag in line.split():
                word = wordWithTag.split('/')
                if word[1] == 'w':
                    self.symbol[word[0]] = 1
                else:
                    if word[0] in self.words:
                        self.words[word[0]] = self.words[word[0]] + 1
                    else:
                        self.words[word[0]] = 1
        data.close()
    
    def readSymbol(self):
        self.symbol.clear()
        data = open("symbol.txt", 'r')
        for line in data.readlines():
            self.symbol[line] = 1
        data.close()
    
    def addDict(self):
        data = open("dictTrain.txt", "r")
        for line in data.readlines():
            word = line.split()
            if word[0] in self.words:
                self.words[word[0]] = self.words[word[0]] + 1
            else:
                self.words[word[0]] = 1
        data.close()
    
    def control(self):
        data = open("test.txt", "r")
#        data = open("dev.txt", "r")
        for line in data.readlines():
            sentence = ""
            counter = 0
            tmp = ""
            for i in line:
                tmp = tmp + str(i)
                counter =  counter + 1
                if counter == 2:
#                    print tmp
                    if tmp in self.symbol:
                        self.divide(sentence)
                        sentence = ""
                        print >> self.result,tmp
                    else:
                        sentence = sentence + tmp
                    counter = 0
                    tmp = ""
            if len(sentence) > 0:
                self.divide(sentence)
        data.close()
        self.result.close()
    
    def divide(self, sentence):
#        print len(sentence)
        wordLen = len(sentence)/2
        
        eachWord = range(wordLen)   #get eachWord
        for i in xrange(wordLen):
            eachWord[i] = str(sentence[i*2]) + str(sentence[i*2+1])
#            print eachWord[i]
            
        maxLen = range(wordLen)     #get the maxLen of each word
        tmpM = 1
        for i in xrange(wordLen):
            tmp = ""
            for j in xrange(i, wordLen):
                tmp = tmp + eachWord[j]
                if tmp in self.words:
                    tmpM = j - i + 1
            maxLen[i] = tmpM
#            print tmpM,
            tmpM = 1

        indexContainer = set()
        for i in xrange(wordLen):
            indexContainer.add(i)
#            print indexContainer
#            indexContainer.discard(i)
        
        posContainer = set()
        while len(indexContainer) > 0:
            maxP = -1
            for i in indexContainer:
                if maxP == -1:
                    maxP = i
                else:
                    tag = 1
                    if maxLen[i] > maxLen[maxP]:
                        for j in posContainer:
                            if i < j and (i + maxLen[i]) > j:
                                tag = 0
                        if tag == 1:
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
        
        for i in xrange(wordLen):
            tmp = ""
            for j in xrange(maxLen[i]):
                tmp = tmp + eachWord[i+j]
            if maxLen[i] > 0:
                print >> self.result, tmp
                
    def showWords(self):
#        print "The words are:"
        out = open("words.txt", "w")
        for i in self.words:
#            print i, self.words[i], len(i)
            print >> out, i
        out.close()
        print
    
    def showSymbol(self):
        print "The symbols are:"
#        out = open("symbol.txt", "w")
        for i in self.symbol:
            print i
#            print >> out, i
#        out.close()
        print

test = divider()
test.readData()
test.addDict()
test.readSymbol()
test.control()
#test.showWords()
#test.showSymbol()

end = clock()
print "The total time is: ", end-start