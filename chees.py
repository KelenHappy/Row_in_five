import cv2
import numpy as np

class const:
    class ConstError(TypeError):pass
    def __setattr__(self,name,value):
        if name in self.__dict__:
            raise self.ConstError("Can't rebind const (%s)" %name)
        self.__dict__[name]=value
       
const.MARGIN = 40
const.GRID = 25
const.NUM = 22
const.LEN = 525
const.WIN = 3

class Chess:
    def __init__(self):
        self.cord = []        
        self.size = 0
    def setXY(self,x: int, y: int, player: int)->bool:
        if x==0 or y==0: return False
        data = [(d[0], d[1]) for d in self.cord]
        if (x, y) not in data:
            self.cord.append((x, y, player))
            self.size += 1
            return True
        return False
    def printChess(self):
        for d in self.cord:
            print(d, end=' ')
        print('size=', self.size)   
        
def drawBoard():
    img = np.zeros((600,600,3), dtype='uint8')   # 繪製 300x300 的黑色畫布
    for i in range(const.NUM):
        cv2.line(img,(const.MARGIN, const.MARGIN+ i*const.GRID),(const.MARGIN+const.LEN, const.MARGIN + i*const.GRID),(0,255,0),1)  # 繪製線條
        cv2.line(img,(const.MARGIN+ i*const.GRID, const.MARGIN),(const.MARGIN + i*const.GRID, const.MARGIN+const.LEN),(0,255,0),1)  # 繪製線條        
    return img

def writeBack(file: str, x:int, y: int):
    f = open(file,'w')
    f.write('W\n')
    f.write(str(x)+' '+str(y))
    f.close()    

def checkWin(data: list):
    count=0
    mark = False
    maxCount = 0
    for d in data:
        if d==0: 
            mark = False
            count=0
        elif (d==1) and (mark ==False):
            mark = True
            count = 1
        elif (d==1) and (mark == True):
            count +=1
        if count>maxCount: maxCount = count    
        
    if maxCount==const.WIN: return True
    return False

#print(checkWin([0,1,1,1,0,0,1,1,1,0,1,1,1,1,1,1,0,1,0]))

def checkBoardColumns(board: list):
    for d in board:                 # 檢查 board 矩陣 直欄是否連續 N 個
        if checkWin(d)==True:
            return True

def checkBoardRows(board: list):    
    for i in range(len(board[0])):  # 檢查 board 矩陣 橫列是否連續 N 個
        t =[d[i] for d in board]    # 將橫列轉換成 list
        if checkWin(t)==True:
            return True   
    
def checkBoardSlashLR(board: list):
    for i in range(len(board)):     # 檢查 board 矩陣，左上斜向右下，下三角，是否連續 N 個
        x, y, t = 0, i, []
        while True:
            if y>=len(board): break
            t.append(board[x][y])   #      
            x += 1
            y += 1
        if checkWin(t)==True:
            return True       

    for i in range(1,len(board)):     # # 檢查 board 矩陣，左上斜向右下，上三角，是否連續 N 個
        x, y, t = i, 0, []
        while True:
            if x>=len(board): break
            t.append(board[x][y])        
            x += 1
            y += 1
        if checkWin(t)==True:
            return True               
    return False

def checkBoardSlashRL(board: list):
    LEN = len(board)
    for i in range(LEN):     # 檢查 board 矩陣，右上斜向左下，下三角，是否連續 N 個
        x, y, t = (LEN-1), i, []
        while True:
            if x<0 or y>=LEN: break
            t.append(board[x][y])   #      
            x -= 1
            y += 1
        if checkWin(t)==True:
            return True       

    for i in range(LEN-1):     # # 檢查 board 矩陣，右上斜向左下，上三角，是否連續 N 個
        x, y, t = i, 0, []
        while True:
            if x<0 or y>=LEN: break
            t.append(board[x][y])        
            x -= 1
            y += 1
        if checkWin(t)==True:
            return True               
    return False
    
def gameOver(chess: Chess, player: int):
    # 取出 chess 物件紀錄的 x, y 座標
    data = [(d[0], d[1]) for d in chess.cord if d[2]==player]
    # 初始化 N*N board 矩陣棋盤，設定為 0
    board =[[0 for j in range(const.NUM)] for i in range(const.NUM)]
    # 將 chess 物件 x, y 資料轉成 N*N board 矩陣棋盤資料，有下棋的位置設定為 1
    for i in range(const.NUM):
        for j in range(const.NUM):
            if (i, j) in data:
                board[i][j]=1
    
    if checkBoardColumns(board)==True: return True    
    if checkBoardRows(board)==True: return True    
    if checkBoardSlashLR(board)==True: return True        
    
    return False

def testGameOver():
    chess = Chess()
    player = 1
    data = [(1, 1, 1), (1, 2, 1), (1, 3, 1), (1, 4, 1), (1, 5, 1), (1, 7, 1)]
    for d in data:
        chess.setXY(d[0], d[1], player)
        
    flag = gameOver(chess, player)
    print(flag, player)    

#testGameOver()    
    
def go(img, fileName: str, px: int, py: int, c: Chess, player: int):
    if player == 0: color = (0, 0, 255)
    else: color = (255, 0, 0)   
        
    f = open(fileName,'r')
    k = f.readline()
    if k[0]=='R':
        k = f.readline()
        data = k.split()
        x, y = int(data[0]), int(data[1])
        print('==>', x, y)
        c.setXY(x, y, player)
        c.printChess()
        cv2.circle(img, (const.MARGIN + x*const.GRID, const.MARGIN + y*const.GRID), 10, color, -1)
        f.close()
        writeBack(fileName, px, py)
        return True, x, y
    f.close()
    return False, 0, 0

def main():
    chess = Chess()
    img = drawBoard()
    fileName = 'a.txt'
    px, py = 0, 0    
    cv2.imshow("image", img)
    while True:        
        if fileName == 'a.txt': player = 0
        else: player = 1
        change, px, py = go(img, fileName, px, py, chess, player)                               
        over = gameOver(chess, player)
        if over == True:
            print(over, player)
            break
        if cv2.waitKey(1000)==27:
            break        
        if change==True and fileName=='a.txt': 
            fileName = 'b.txt'
        elif change==True and fileName=='b.txt': 
            fileName = 'a.txt'
        if change == True:
            cv2.imshow("image", img)
        
    cv2.imshow("image", img)
    cv2.waitKey(18000)
    cv2.destroyAllWindows()            

main()    