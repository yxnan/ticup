 commandStr = './receive';
 [status, commandOut] = system(commandStr);
 if status==0
     fftshow(commandOut)
 end