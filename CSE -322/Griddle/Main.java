import java.io.File;
import java.io.FileNotFoundException;
import java.util.*;

public class Main {

    private static int cost(int[][][] l, int[] w){
        int p=l.length;
        int q = l[0].length;
        int[] cost_={0,0,0};
        for (int i = 0; i < p; i++) {
            for (int j = 0; j < q-1; j++) {
                for (int k = j+1; k < q; k++) {
                    for (int a = 0; a < 3; a++) {
                        if(l[i][j][a]==l[i][k][a] && l[i][j][a]!=-2){
                            cost_[a]+=w[a];
                        }
                    }
                }
            }
        }
        int cost__=0;
        for (int i = 0; i < 3; i++) {
            cost__+=cost_[i];
        }
        return cost__;
    }

    private static void swap(int[][][] l, int i,int j, int i_, int j_){
        int[] temp=l[i][j];
        l[i][j]=l[i_][j_];
        l[i_][j_]=temp;
    }

    private static int[][][] copy(int[][][] source){
        int[][][] dest=new int[source.length][source[0].length][3];
        for (int i = 0; i < source.length; i++) {
            for (int j = 0; j < source[0].length; j++) {
                dest[i][j]=source[i][j].clone();
            }
        }
        return dest;
    }

    private static void store(int[][][] l){
        try {
            for (int i = 0; i < l.length; i++) {
                for (int j = 0; j < l[0].length; j++) {
                    for (int k = 0; k < 3; k++) {
                        System.out.print(l[i][j][k]+",");
                    }
                    System.out.print(" ");
                }
                System.out.println();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private static int hillClimb(int[][][] l,int[] w){
        int min=cost(l,w);
        if(min==0){
            //store(l);
            return 0;
        }
        //System.out.println("Initial Cost : "+min);
        int p,q;
        p=l.length;
        q=l[0].length;
        int[][][] temp;
        temp=copy(l);
        while(true){
            int flag=0;
            for (int i = 0; i < p-1; i++) {
                for (int j = 0; j < p ; j++) {
                    for (int k = 0; k < q; k++) {
                        for (int m = 0; m < q; m++) {
                            swap(temp,i,k,j,m);
                            int c = cost(temp,w);
                            if(c<min){
                                min=c;
                                l=copy(temp);
                                if(min==0){
                                    return 0;
                                }
                                flag=1;
                            }
                            swap(temp,i,k,j,m);
                        }
                    }
                }
            }
            if(flag==0){
                return min;
            }
            temp=copy(l);
        }
    }

    public static void main(String[] args) {
        long startTime = System.currentTimeMillis();
        String[] fname={"hdtt","note.txt", "req.txt"};
        System.out.print("Enter File Number : ");
        Scanner scan = new Scanner(System.in);
        String sp = scan.next();
        scan.close();
        File file = new File(fname[0]+sp+fname[1]);
        int[] info=new int[5];
        String s;
        try {
            scan = new Scanner(file);
            int i=0;
            String[] si;-version
            while (scan.hasNextLine()) {
                s=scan.nextLine();
                si=s.split(" ");
                info[i++]=Integer.parseInt(si[si.length-1]);
            }
            scan.close();
        }
        catch (FileNotFoundException e) {
            e.printStackTrace();
        }
        file = new File(fname[0]+sp+fname[2]);

        //----------------------------------------WEIGHT------------------------
        int[] w={1,1,1};
        //---------------------------------------PERIODS------------------------
        int periods=40;


        int cost=10000;
        do{
            int[][][] R=new int[info[3]][periods][3];
            int[] count =new int[info[3]];
            for (int i = 0; i < info[3]; i++) {
                for (int j = 0; j < periods; j++) {
                    for (int k = 0; k < 3; k++) {
                        R[i][j][k]=-2;
                    }
                }
            }
            try {
                String[] si;
                scan = new Scanner(file);
                int i=0,ln=0,tch=0,cls=0;
                while(scan.hasNext()){
                    int p=scan.nextInt();
                    for (int j = 0; j < p; j++) {
                        R[i][count[i]][0]=tch;
                        R[i][count[i]][1]=cls;
                        R[i][count[i]++][2]=i;
                    }
                    ln++;
                    tch++;
                    if(tch==info[0]){ tch=0;cls++; }
                    if(ln==info[3]*info[3]){ ln=0;i++;cls=0; }
                }
                scan.close();
            }
            catch (FileNotFoundException e) {
                e.printStackTrace();
            }
            int[][][] l=new int[periods][info[3]][3];
            Random ran = new Random();
            //System.out.println("New");
            //store(R);
            for (int i = 0; i < periods; i++) {
                for (int j = 0; j < info[3]; j++) {
                    int r = R[j].length;
                    int p = ran.nextInt(r);
                    while (R[j][p][0] == -1) {
                        p = ran.nextInt(r);
                    }
                    int[] A = R[j][p];
                    l[i][j] = A.clone();
                    R[j][p][0] = -1;
                }
            }
            int cost1=hillClimb(l, w);
            if(cost1<cost){
                cost=cost1;
                System.out.println(cost);
            }
            System.out.println("--- " + (System.currentTimeMillis() - startTime) + " milliseconds ---");
        }while(cost!=0);
    }
}
