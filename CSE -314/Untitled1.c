https://github.com/QinMing/Nachos-Operating-System/blob/master/code/userprog/addrspace.cc

int AddrSpace::loadIntoFreePage(int addr, int physicalPageNo){
    // here addr is the bad address
    // physicalPageNo has already been found by the memoryManager Alloc function
    int vpn= addr/PageSize;

    pageTable[vpn].physicalPage = physicalPageNo;
    pageTable[vpn].valid = true;

    pageTable[vpn].use = false;
    pageTable[vpn].dirty = false;
    pageTable[vpn].readOnly = false;

    memoryLock->Acquire();

    if(backingStore->isPresent(&pageTable[vpn]))
    // load from backing store
    {

    }

    else{  // load from executable

        // first time page fault
        int iPage = vpn*PageSize;
        int fPage = iPage + PageSize - 1;
        int fCode = noffH.code.virtualAddr+noffH.code.size-1;
        int fData = noffH.initData.virtualAddr+noffH.initData.size-1;
        printf("iPage: %d\n", iPage);
        printf("fPage: %d\n", fPage);
        printf("noffH.code.virtualAddr: %d\n", noffH.code.virtualAddr);
        printf("noffH.code.size: %d\n", noffH.code.size);
        printf("noffH.initData.virtualAddr: %d\n", noffH.initData.virtualAddr);
        printf("noffH.initData.size: %d\n", noffH.initData.size);
        printf("noffH.uninitData.virtualAddr: %d\n", noffH.uninitData.virtualAddr);
        printf("noffH.uninitData.size: %d\n", noffH.uninitData.size);
        printf("fCode: %d\n", fCode);
        printf("fData: %d\n", fData);
        // now 6 cases
        if(noffH.code.size>0 && iPage>=noffH.code.virtualAddr
            && iPage<=fCode)
        {
            // page starts in code segment
            // 3 cases
            printf("page starts in code segment and ");
            //int fCode = noffH.code.virtualAddr+noffH.code.size-1;
            if(fPage<=fCode){
                // page ends in code segment
                printf("ends in code segment\n");
                int codeoffset = iPage - noffH.code.virtualAddr;
                executable->ReadAt(&(machine->mainMemory[physicalPageNo * PageSize]),
                    PageSize, noffH.code.inFileAddr + codeoffset);
            }
            else if(noffH.initData.size>0
                && fPage<=fData)
            {
                // page ends in initData segment
                printf("ends in initData segment\n");
                int codeoffset = iPage - noffH.code.virtualAddr;
                int codesize = fCode-iPage+1;
                executable->ReadAt(&(machine->mainMemory[physicalPageNo * PageSize]),
                    codesize, noffH.code.inFileAddr + codeoffset);
                int datasize = PageSize - codesize;
                executable->ReadAt(&(machine->mainMemory[physicalPageNo * PageSize+codesize]),
                    datasize, noffH.initData.inFileAddr);
            }
            else{
                // page ends in uninitData segment or stack segment
                printf("ends in uninitData or stack segment\n");
                int codeoffset = iPage - noffH.code.virtualAddr;
                int codesize = fCode-iPage+1;
                executable->ReadAt(&(machine->mainMemory[physicalPageNo * PageSize]),
                    codesize, noffH.code.inFileAddr + codeoffset);

                int datasize = 0;  // if the size of initData segment is 0
                if(noffH.initData.size>0){
                    // so the base address is valid
                    datasize = noffH.initData.size;  // whole initData segment
                    executable->ReadAt(&(machine->mainMemory[physicalPageNo * PageSize+codesize]),
                        datasize, noffH.initData.inFileAddr);
                }
                int remaining = PageSize-codesize-datasize;
                bzero(&machine->mainMemory[physicalPageNo * PageSize+codesize+datasize], remaining);
            }
        }

        else if(noffH.initData.size>0 && iPage>=noffH.initData.virtualAddr
            && iPage<=fData){
            // page starts in initData segment
            // 2 cases
            printf("page starts in initData segment and ");
            //int fData = noffH.initData.virtualAddr+noffH.initData.size-1;
            if(fPage<=fData){
                // page ends in initData segment
                printf("ends in initData segment\n");
                int dataoffset = iPage - noffH.initData.virtualAddr;
                executable->ReadAt(&(machine->mainMemory[physicalPageNo * PageSize]),
                    PageSize, noffH.initData.inFileAddr + dataoffset);
            }
            else{
                // page ends in uninitData segment or stack segment
                printf("ends in uninitData or stack segment\n");
                int dataoffset = iPage - noffH.initData.virtualAddr;
                int datasize = fData-iPage+1;
                executable->ReadAt(&(machine->mainMemory[physicalPageNo * PageSize]),
                    datasize, noffH.initData.inFileAddr + dataoffset);
                int uninitdatasize = PageSize - datasize;
                bzero(&machine->mainMemory[physicalPageNo * PageSize+datasize], uninitdatasize);

            }
        }
        else {
            // page starts in uninitData segment or stack segment
            printf("page starts in uninitData segment or stack segment\n");
            bzero(&machine->mainMemory[physicalPageNo * PageSize], PageSize);
        }

    }

    memoryLock->Release();
    return 0;
}
