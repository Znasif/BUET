import numpy as np
#import matplotlib.pyplot as plt

# WHITE is -1 (Player 2)
# BLACK is 1 (Player 1)
total_moves=-1

point = np.array([[30, -25, 10, 5, 5, 10, -25, 30],
                  [-25, -25, 1, 1, 1, 1, -25, -25],
                  [10, 1, 5, 2, 2, 5, 1, 10],
                  [5, 1, 2, 1, 1, 2, 1, 5],
                  [5, 1, 2, 1, 1, 2, 1, 5],
                  [10, 1, 5, 2, 2, 5, 1, 10],
                  [-25, -25, 1, 1, 1, 1, -25, -25],
                  [30, -25, 10, 5, 5, 10, -25, 30]])


def initial_board(size):
    board=np.zeros((size,size))
    q=size//2
    p=q-1
    board[p][p]=-1
    board[q][q]=-1
    board[p][q]=1
    board[q][p]=1
    return board


def who_wins(board):
    a = np.sum(board)
    if(a>0):
        return 1
    elif(a<0):
        return -1
    return 0


def make(board):
    return tuple(tuple(i) for i in board)


# def printf(board,title):
#     plt.imshow(board)
#     plt.grid()
#     plt.title(title)
#     ax=plt.gca()
#     ax.set_xticks(np.arange(-.5, 8, 1))
#     ax.set_yticks(np.arange(-.5, 8, 1))
#     ax.set_xticklabels(np.arange(0, 8, 1))
#     ax.set_yticklabels(np.arange(0, 8, 1))
#     plt.show()


def printf(board,title):
    global total_moves
    total_moves+=1
    print(title)
    print(board)


def modify(b,row,col,p,r):
    board=np.copy(b)
    size,_=np.shape(board)
    board[row][col]=p
    go=[row,col,size-row-1,size-col-1,min(row,col),min(col,size-row-1),min(size-row-1,size-col-1),min(size-col-1,row)]
    line=[(-1,0),(0,-1),(1,0),(0,1),(-1,-1),(1,-1),(1,1),(-1,1)]
    flag=[0 for i in range(8)]
    cnt=[0 for i in range(8)]
    valid_move=0
    for i in range(8):
        a1,b1=line[i]
        for j in range(1,go[i]+1):
            a,b=j*a1,j*b1
            if(board[row+a][col+b]==p):
                if(cnt[i]>0):
                    flag[i]=1
                break
            if(board[row+a][col+b]==0):
                break
            else:
                cnt[i]+=1
    if(r):
        return sum(flag)>0
    for i in range(8):
        if(flag[i]==1):
            a1,b1=line[i]
            for j in range(1,cnt[i]+1):
                a,b=j*a1,j*b1
                valid_move=1
                board[row+a][col+b]=p

    return valid_move,board


def can(board,player):
    size,_=np.shape(board)
    p=0
    for i in range(size):
        for j in range(size):
            if(board[i][j]==0 and modify(board,i,j,player,True)):
                p +=1
    return p>0


def not_leaf(board):
    return can(board,1) or can(board,-1)


def evaluate_table(board, player):
    if(player==1):
        return np.sum(board*point)
    return -np.sum(board*point)


def evaluate_mobility(b, player):
    p=[(0,0),(0,7),(7,0),(7,7)]
    p1,p2=[0,0],[0,0]
    board=np.copy(b)
    size,_=np.shape(board)
    for i in range(4):
        a,b=p[i]
        if(board[a][b]==player):
            p1[0]+=1
        elif(board[a][b]==-player):
            p2[0]+=1
    for i in range(size):
        for j in range(size):
            if(board[i][j]==0 and modify(board,i,j,player,True)):
                p1[1]+=1
            if(board[i][j]==0 and modify(board,i,j,-player,True)):
                p2[1]+=1
    if(p1[1]+p2[1]!=0):
        return 10*(p1[0]-p2[0])+(p1[1]-p2[1])/(p1[1]+p2[1])
    return 0


def evaluate_count(board,player):
    if(player==1):
        return np.sum(board)
    return -np.sum(board)


def max_p1(board,alpha,beta,depth):
    a,b=can(board,1), can(board,-1)
    size,_=np.shape(board)
    if((a or b)==False):
        return who_wins(board)
    if(depth==0):
        return evaluate_count(board,1)
        # return evaluate_mobility(board,1)
        # return evaluate_table(board, 1)
    mx=-np.inf
    global Boards,Level
    if(a):
        for i in range(size):
            for j in range(size):
                if(board[i][j]==0):
                    valid_move,new_board=modify(board,i,j,1,False)
                    if(valid_move):
                        cmin=min_p2(new_board,alpha,beta,depth-1)
                        if(cmin>mx):
                            mx=cmin
                            Boards[make(board)]=new_board
                            Level[make(board)]=1
                        if(mx>=beta): return mx
                        alpha=max(alpha,mx)
        return mx
    else:
        #print("No valid Move")
        return min_p2(board,alpha,beta,depth)


def min_p2(board,alpha,beta,depth):
    a,b=can(board,1), can(board,-1)
    size,_=np.shape(board)
    if((a or b)==False):
        return who_wins(board)
    if(depth==0):
        return evaluate_count(board,-1)
        # return evaluate_mobility(board,-1)
        # return evaluate_table(board, -1)
    mn=np.inf
    global Boards,Level
    if(b):
        for i in range(size):
            for j in range(size):
                if(board[i][j]==0):
                    valid_move,new_board=modify(board,i,j,-1,False)
                    if(valid_move):
                        cmax=max_p1(new_board,alpha,beta,depth-1)
                        if(cmax<mn):
                            mn=cmax
                            Boards[make(board)]=new_board
                            Level[make(board)]=2
                        if(mn<=beta): return mn
                        beta=min(beta,mn)
        return mn
    else:
        #print("No valid Move")
        return max_p1(board,alpha,beta,depth)


if __name__ == "__main__":
    SIZE=8
    DEPTH=int(input("Input Depth : "))
    b = initial_board(SIZE)
    printf(b,"Initial Board")
    Boards={}
    Level={}
    max_p1(b,-np.inf,np.inf,DEPTH)
    prev_board=Boards[make(b)]
    prev_move=Level[make(b)]
    printf(prev_board,"Move by Player : "+str(prev_move))
    while(not_leaf(prev_board)):
        if(make(prev_board) not in Level):
            if(prev_move==1):
                min_p2(prev_board,-np.inf,np.inf,DEPTH)
            elif(prev_move==2):
                max_p1(prev_board,-np.inf,np.inf,DEPTH)
        prev_move=Level[make(prev_board)]
        prev_board=Boards[make(prev_board)]
        printf(prev_board,"Move by Player : "+str(prev_move))
    a,b=0,0
    for i in range(SIZE):
        for j in range(SIZE):
            if(prev_board[i][j]==1):
                a+=1
            elif(prev_board[i][j]==-1):
                b+=1
    print("Score Player 1 :"+str(a))
    print("Score Player 2 :"+str(b))
    print("Total Number of Moves :" +str(total_moves))
    if(a>b):
        print("Winner is Player 1")
    elif(a<b):
        print("Winner is Player 2")
    else:
        print("Match ended in a Draw")
