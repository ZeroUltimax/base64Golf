#pragma once

const char*y="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
int w(const char*d){for(int i=0;;i++)if(y[i]==*d)return i;}

// Both algorithms work with these facts:
// Every 3 byte is stored in chunks of 4 chars in d
// Every byte overlaps a pair of char.
// Every algorithm has 3 configurations, depending on which of the 3 pair is overlapped
// When masking base64 char's value, we only care about the 6lsb of the mask. So \0 and @ mask those the same way.

// get the ith byte stored in the base64 string d
// 1- Select the configuration
// 2- We index that pair in d
// 3- for each of the 2 overlapped char
//     * We get the char
//     * We convert the char to it's base64 value (Ex.: C -> 2)
//     * We mask the bits that we need for the final byte
//     * We shift the bits so they are in their final position
// 4- We OR the two parts of the final byte
int base64get(const char*d,int i){
    const char*t="\2?C\4\4OO\2\6C?"+i%3*4;
    return (w(d+=i*4/3)&t[1])<<*t|w(d+1)>>t[3]&t[2];
}

// set the ith byte  in the base64 string d to v
// 1- Select the configuration
// 2- We index that pair in d
// 3- For each of the 2 chars where the byte overlaps
//     * We get the char
//     * We convert the char to it's base64 value (Ex.: C -> 2)
//     * We mask the char value to only keep the bits that are not affected by the byte
//     * We mask the bits of the byte to only keep those overlapping the char
//     * We shift the masked bits of the byte to align it with the char
//     * We set the char to the OR of the unnafected bits and the masked/shift bits of the byte
void base64set(char*d,int i,int v){
    const char*t="\2O@0\4\4Cp<\2\6@<?"+i%3*5;
    *d++=y[w(d+=i*4/3)&t[2]|v>>*t];
    *d=y[w(d)&t[1]|v<<t[4]&t[3]];
}