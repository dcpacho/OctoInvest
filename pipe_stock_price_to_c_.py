TICS=["INDI","EAF","CNTX","BROS","RCUS","INTC","WMT","SKYW","APVO","AAPL","GOOG","HD","LOW","MRIN","CERO","INDI","MNKD","GT","META","DG","MCD","IBM","MSFT","PEP","AAL","KHC"]
TICKER_COUNT = 17#len(TICS)
import random as rd
rd.shuffle(TICS)
import sys; print(f"running in: {sys.platform}")
if sys.platform == "win32":
    import yfinance as yf
   # import time as t
    import struct as s
    import win32pipe as wp
    import win32file as wf
    #configure named pipe
    PID = r"\\.\pipe\ -<{( Hit This Pipe )}>-"; print(f"Creating named pipe: {PID}")
    PIPE = wp.CreateNamedPipe(PID,wp.PIPE_ACCESS_OUTBOUND, wp.PIPE_TYPE_BYTE | wp.PIPE_WAIT, 1, 65536, 0, 0, None); print("Waiting for client connection...")
    wp.ConnectNamedPipe(PIPE, None); print("Client connected!")
    packed_data = s.pack('i', TICKER_COUNT)
    #send number of tickers to expect
    wf.WriteFile(PIPE,packed_data);print(f"Sent: {packed_data}")
    #send stock prices in an infinite loop every 120 seconds
    #while True:
    for d in range(-100, 0):
        try:
            #fetch and send each stock price
            for i in range(0, TICKER_COUNT ):
                try: DATA = yf.Ticker( str(TICS[ i ]) ).history(period="6mo", interval="1d")['Close'].iloc[d]
                except (KeyError, IndexError, ValueError) as e:
                    print(f"Error fetching data for ticker {TICS[i]}: {e}. Retrying.")
                    try: DATA = yf.Ticker( str(TICS[ i ]) ).history(period="6mo", interval="1d")['Close'].iloc[d]
                    except (KeyError, IndexError, ValueError) as f:
                        print(f"Error fetching data for ticker {TICS[i]}: {f}. Using 0.0.")
                        DATA = 0.0
                packed_data = s.pack('d', DATA)  # Pack as double (8 bytes)
                wf.WriteFile(PIPE, packed_data)
                print(f"Sent{TICS[i]} #{i} : {DATA}")
            #t.sleep(120)
        except KeyboardInterrupt: 
            wf.CloseHandle(PIPE)
            break
        except (wp.error, wf.error, s.error) as e:
            wf.CloseHandle(PIPE)
            print(f"Error during communication: {e}")
            break
else:
    print("This script is only supported on Windows. please add pipeline it for your OS and adjust this error code.")
    sys.exit(1)
        