enum State
{
  CONNECTED, DISCONNECTED, RECORDING, PLAYING, STANDBY, READY, NOTREADY
};

State     connectState  = DISCONNECTED;
State     recordState   = STANDBY;
State     readyState;


int       mode          = 0;                // 0 = idle; 1 = record; 2 = play


String stateToText( int state )             // for Serial monitor diagnostics
{
  String value = "";
  switch ( state )
  {
    case CONNECTED :
      value = "CONNECTED";
      break;
    case DISCONNECTED :
      value = "DISCONNECTED";
      break;
    case RECORDING :
      value = "RECORDING";
      break;
    case PLAYING :
      value = "PLAYING";
      break;
    case STANDBY :
      value = "STANDBY";
      break;
    case READY :
      value = "READY";
      break;
    case NOTREADY :
      value = "NOTREADY";
      break;
    default:
      value = "unknown state";
      break;
  }
  return value;
}
