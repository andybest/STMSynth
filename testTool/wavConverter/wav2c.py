import wave
import struct
import argparse
import sys


def main(argv):
    parser = argparse.ArgumentParser(description='Convert wav files to a C array of floats')
    parser.add_argument('wavefile', metavar='W', type=str, nargs='+', help='wave file to convert')
    parser.add_argument('outputfile', metavar='O', type=str, nargs='+', help='output file')
    parser.add_argument('arrayname', metavar='A', type=str, nargs='+', help='array name')
    args = parser.parse_args()
    
    inputFile = args.wavefile[0]
    outputFile = args.outputfile[0]
    arrayName = args.arrayname[0]
    
    convert(inputFile, outputFile, arrayName)

def convert(wavfile, outputfile, arrayname):
    w = wave.open(wavfile, 'r')
    
    numFrames = w.getnframes()
    convertedSamples = []
    
    while(numFrames > 0):
        samples = w.readframes(1)
        
        sample = struct.unpack('<h', samples)
        floatSample = sample[0] / 32768.0
        convertedSamples.append(floatSample)
        numFrames -= 1
        
    w.close()
    
    cSource = "float " + arrayname + "[" + str(len(convertedSamples)) + "] = {\n"
    cSource += "\t"
    
    rowCount = 0
    
    for i in xrange(0, len(convertedSamples)):
        cSource += str(convertedSamples[i])
        
        if i < len(convertedSamples) - 1:
            cSource += ", "
        
        rowCount += 1
        if rowCount == 4:
            cSource += "\n\t"
            rowCount = 0
    cSource += "\n};\n"
        
    with open(outputfile, 'w') as f:
        f.write(cSource)


if __name__ == '__main__':
    main(sys.argv[1:])