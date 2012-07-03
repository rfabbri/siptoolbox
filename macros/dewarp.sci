function Img=dewarp(img)
//
// Reads any image into scilab and dewarp's it.
//
// AUTHOR
//    Ricardo Fabbri  <rfabbri@(not this part) gmail d0t com>
//    Cybernetic Vision Research Group
//    Luciano da Fontoura Costa, supervisor.
//    http://siptoolbox.sf.net
//
//    Pallan Madhavan <mady902@(not this part) gmail d0t com>
//    Integrated Masters Degree (Part V)
//    IIT(BHU)-Varanasi
// $Revision: 1.2 $ $Date: 2012-07-03 19:23:48 $
   imwrite(img,'/tmp/sip.jpg')
   IMg = dewarp_c('/tmp/sip.jpg')
   Img = IMg/255
endfunction
