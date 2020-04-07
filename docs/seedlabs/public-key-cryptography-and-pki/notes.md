# Public-Key Cryptography and PKI

## Questions to answer

1. What is a Certificate Authority (CA), and how to become one?
2. How to create a certificate for a customer? (i.e. www.ruihan.org)
3. How PKI is used for web site authentication and encryption?
4. What are the necessary **files** of such an infrastructure?

## 0x1 Become a Certificate Authority (CA)

CA is a trusted entity to issue digital certificates. The digital certificate
certifies the ownership of a public key by the named subject of the certificate.
Root CAs are a number of trusted commercial CAs. (VeriSign is one of the largest)
However, we are not obligate to the commercial world, we are free to become a
root CA, and then use this root CA to issue certificates to others. Remember that
the root CAs are self-signed. root CA's certificates are pre-loaded into most
OS, browsers, and other software that using PKI. Root CA's certificates are
**unconditionally** trusted.

To create a root CA, we use the following openssl command to generate a
self-signed certificate for the CA.

```text
seed@ubuntu:~/SEED-Labs/PKI-Lab$ openssl req -new -x509 -keyout ca.key -out ca.crt -config openssl.cnf
Generating a 1024 bit RSA private key
..........................++++++
..++++++
unable to write 'random state'
writing new private key to 'ca.key'
Enter PEM pass phrase:
Verifying - Enter PEM pass phrase:
-----
You are about to be asked to enter information that will be incorporated
into your certificate request.
What you are about to enter is what is called a Distinguished Name or a DN.
There are quite a few fields but you can leave some blank
For some fields there will be a default value,
If you enter '.', the field will be left blank.
-----
Country Name (2 letter code) [AU]:US
State or Province Name (full name) [Some-State]:CA
Locality Name (eg, city) []:San Jose
Organization Name (eg, company) [Internet Widgits Pty Ltd]:PKILabServer
Organizational Unit Name (eg, section) []:
Common Name (e.g. server FQDN or YOUR name) []:PKILabServer.com
Email Address []:rui@PKILabServer.com
[11/17/2016 11:14] seed@ubuntu:~/SEED-Labs/PKI-Lab$
```

The Configure File ```openssl.conf``` is used by three OpenSSL command:
```ca```, ```req```, and ```x509```. To get a glimpse of it's purporse, here is
partial of the ```openssl.conf``` (from [CA_Default} section).

```text
dir           = ./demoCA        # Where everything is kept
certs         = $dir/certs      # Where the issued certs are kept
crl_dir       = $dir/crl        # Where the issued crl are kept
new_certs_dir = $dir/newcerts   # default place for new certs
database      = $dir/index.txt  # database index file
serial        = $dir/serial     # The current serial number
```

## 0x2 Create a Certificate for **PKILabServer.com**

When one becomes a certificate autority(CA), he/she is ready to sign digital
certificates for his/her customers. ```PKILabServer.com``` is a company that
tries to get a digital certificate from a CA. He needs to do the following steps:

>1. Generate RSA key pairs
>2. Generate a certificate signing request (CSR)
>3. Generate a certificate

### 0x21 Generate RSA Key Paris

This step could be done using OpenSSL command line tool like this:

```text
seed@ubuntu:~/SEED-Labs/PKI-Lab$ openssl genrsa -aes128 -out server.key 1024
Generating RSA private key, 1024 bit long modulus
....++++++
.............++++++
unable to write 'random state'
e is 65537 (0x10001)
Enter pass phrase for server.key:
Verifying - Enter pass phrase for server.key:
[11/17/2016 11:16] seed@ubuntu:~/SEED-Labs/PKI-Lab$
```

The generated keys will be stored in the file ```server.key```. This file is
encrypted as indicated by the option ```-ase128```, which means using AES-128
encrytpion algorithm. The effect of this is you have to enter a password when
executing the above command. To see the plan text content of the
```server.key``` file, you can run the following command:

```text
seed@ubuntu:~/SEED-Labs/PKI-Lab$ openssl rsa -in server.key -text
Enter pass phrase for server.key:
Private-Key: (1024 bit)
modulus:
    00:e1:23:f9:75:2d:2c:d9:21:36:6f:62:d6:28:6a:
    2c:c6:1a:3f:d0:77:c2:8c:e0:49:1a:95:b8:99:6d:
    7f:15:cb:db:9c:42:6e:a9:c7:fd:af:cd:94:3e:b5:
    d3:48:1a:14:11:ca:a2:63:d6:27:32:bc:db:75:d7:
    5e:05:9a:8c:2b:7d:0d:fc:6f:61:77:69:c3:37:6b:
    94:a3:d0:5b:70:0d:80:9a:89:ea:10:04:cd:6f:4a:
    72:b9:e7:d6:1b:fa:ff:25:a1:10:0e:05:d5:14:c7:
    34:97:78:55:0b:c7:ed:c4:16:7c:0b:fa:df:46:dd:
    7f:35:00:7f:72:21:02:38:51
publicExponent: 65537 (0x10001)
privateExponent:
    00:bb:06:91:72:1f:c7:03:d0:ad:51:b6:7f:45:2f:
    7e:66:4a:e3:fc:1f:a0:84:3a:c5:3f:ca:64:81:40:
    0e:b4:62:96:da:a5:ed:1c:29:40:ba:cc:42:7c:1d:
    bb:98:47:e1:a8:2d:f6:dc:8c:c8:1f:43:f7:e3:5a:
    b7:86:5c:2e:57:5a:ea:fa:8b:48:28:6a:4c:35:6b:
    fe:80:83:1f:9b:44:80:73:98:62:e2:bc:7e:e1:f0:
    15:dd:74:dc:05:e0:5d:a5:59:a4:71:5f:96:13:e5:
    8f:a6:4c:c0:31:50:ca:b1:b9:e4:92:de:e1:e9:2a:
    2b:3a:2e:07:1c:fb:dc:ac:01
prime1:
    00:f9:bf:6b:f0:28:15:b5:ba:2e:5f:c1:62:63:d7:
    53:b0:ab:9c:3f:53:e1:9b:76:1c:12:fb:b0:61:c4:
    34:fa:28:d7:94:d2:d3:d9:fe:e5:84:57:60:77:23:
    af:82:75:50:d6:0c:2e:2e:23:55:82:8c:6f:5c:b4:
    5a:2f:77:82:b1
prime2:
    00:e6:c6:d9:bf:57:3b:a4:1e:b4:62:c9:fd:5f:e9:
    d8:cc:d7:49:9e:13:db:99:2c:99:2e:7e:78:0a:c2:
    b1:b4:c4:2f:08:fc:76:89:e2:60:12:7f:b7:47:bb:
    d2:98:1e:03:99:42:d7:ec:ab:0d:55:52:95:5e:b8:
    77:c5:55:37:a1
exponent1:
    63:09:e2:fe:f0:96:73:63:6a:a2:74:68:d5:18:fd:
    ca:30:b3:9c:75:62:21:29:3c:46:d6:e2:82:52:b1:
    83:86:90:bf:26:bf:f6:51:db:a8:98:91:db:8d:1e:
    3b:88:d4:4e:9d:b3:ee:7d:fa:99:f3:a0:f1:cd:5f:
    7a:35:55:71
exponent2:
    00:b9:05:4e:48:80:a8:b9:71:40:90:3f:7f:5b:a9:
    81:7e:e1:50:0d:63:c0:58:f3:0f:b0:de:06:62:22:
    2c:15:50:80:a1:44:bf:c8:d6:6c:ce:08:05:2a:86:
    e4:55:bf:22:85:7a:b8:e0:ef:56:d6:44:4a:ab:51:
    5b:fd:22:d0:61
coefficient:
    3d:ec:20:c8:23:3e:d9:f3:88:8f:03:9d:1b:57:c8:
    9d:87:14:83:a2:a8:2c:bc:cc:e7:dd:2d:c4:ef:74:
    2a:21:2b:5a:ef:fb:79:49:f0:bc:6f:4e:d3:0e:f7:
    d2:48:af:b1:12:ae:43:e6:1b:03:bb:f2:18:f1:61:
    8b:7a:1e:8f
writing RSA key
-----BEGIN RSA PRIVATE KEY-----
MIICXQIBAAKBgQDhI/l1LSzZITZvYtYoaizGGj/Qd8KM4EkalbiZbX8Vy9ucQm6p
x/2vzZQ+tdNIGhQRyqJj1icyvNt1114FmowrfQ38b2F3acM3a5Sj0FtwDYCaieoQ
BM1vSnK559Yb+v8loRAOBdUUxzSXeFULx+3EFnwL+t9G3X81AH9yIQI4UQIDAQAB
AoGBALsGkXIfxwPQrVG2f0UvfmZK4/wfoIQ6xT/KZIFADrRiltql7RwpQLrMQnwd
u5hH4agt9tyMyB9D9+Nat4ZcLlda6vqLSChqTDVr/oCDH5tEgHOYYuK8fuHwFd10
3AXgXaVZpHFflhPlj6ZMwDFQyrG55JLe4ekqKzouBxz73KwBAkEA+b9r8CgVtbou
X8FiY9dTsKucP1Phm3YcEvuwYcQ0+ijXlNLT2f7lhFdgdyOvgnVQ1gwuLiNVgoxv
XLRaL3eCsQJBAObG2b9XO6QetGLJ/V/p2MzXSZ4T25ksmS5+eArCsbTELwj8doni
YBJ/t0e70pgeA5lC1+yrDVVSlV64d8VVN6ECQGMJ4v7wlnNjaqJ0aNUY/cows5x1
YiEpPEbW4oJSsYOGkL8mv/ZR26iYkduNHjuI1E6ds+59+pnzoPHNX3o1VXECQQC5
BU5IgKi5cUCQP39bqYF+4VANY8BY8w+w3gZiIiwVUIChRL/I1mzOCAUqhuRVvyKF
erjg71bWREqrUVv9ItBhAkA97CDIIz7Z84iPA50bV8idhxSDoqgsvMzn3S3E73Qq
ISta7/t5SfC8b07TDvfSSK+xEq5D5hsDu/IY8WGLeh6P
-----END RSA PRIVATE KEY-----
[11/17/2016 11:57] seed@ubuntu:~/SEED-Labs/PKI-Lab$
```

The password you entered works like a key to decrypt the ciphertext in the file.

### 0x22 Generate a certificate signing request (CSR)

To request a certificate, the company need to include its public key in a
certificate signing request (CSR) and send the CSR to the certificate authority.
The command to generate the CSR using OpenSSL is:

```text
seed@ubuntu:~/SEED-Labs/PKI-Lab$ openssl req -new -key server.key -out server.csr -config openssl.cnf
Enter pass phrase for server.key:
You are about to be asked to enter information that will be incorporated
into your certificate request.
What you are about to enter is what is called a Distinguished Name or a DN.
There are quite a few fields but you can leave some blank
For some fields there will be a default value,
If you enter '.', the field will be left blank.
-----
Country Name (2 letter code) [AU]:US
State or Province Name (full name) [Some-State]:CA
Locality Name (eg, city) []:San Jose
Organization Name (eg, company) [Internet Widgits Pty Ltd]:PKILabServer
Organizational Unit Name (eg, section) []:
Common Name (e.g. server FQDN or YOUR name) []:PKILabServer.com
Email Address []:admin@PKILabServer.com

Please enter the following 'extra' attributes
to be sent with your certificate request
A challenge password []:server.csr
An optional company name []:
[11/17/2016 11:19] seed@ubuntu:~/SEED-Labs/PKI-Lab$
```

Note that this command is quite similar to the one we used in creating a 
self-signed certificate for the certificate authority previously. The only
difference is the ```-x509``` option.

### 0x23 Generate a certificate

The CSR file needs to have the CA's signature to form a certificate. When
received the CSR file, CA will first verify the identity information in the CSR.
In reality, CA might contact the company through physical channel to do the
verification. For example, through traditional mails or letters. For this
experiment, we can assume we are the CA and will generate the certificate for
the company. The following command turns the certificate signing request
(```server.csr```) into an X509 certificate (```server.crt```), using the CA's
```ca.crt``` and ```ca.key```:

```text
seed@ubuntu:~/SEED-Labs/PKI-Lab$ openssl ca -in server.csr -out server.crt -cert ca.crt -keyfile ca.key -config openssl.cnf
Using configuration from openssl.cnf
Enter pass phrase for ca.key:
Check that the request matches the signature
Signature ok
Certificate Details:
        Serial Number: 4096 (0x1000)
        Validity
            Not Before: Nov 17 19:21:33 2016 GMT
            Not After : Nov 17 19:21:33 2017 GMT
        Subject:
            countryName               = US
            stateOrProvinceName       = CA
            localityName              = San Jose
            organizationName          = PKILabServer
            commonName                = PKILabServer.com
            emailAddress              = admin@PKILabServer.com
        X509v3 extensions:
            X509v3 Basic Constraints: 
                CA:FALSE
            Netscape Comment: 
                OpenSSL Generated Certificate
            X509v3 Subject Key Identifier: 
                B4:F1:39:2A:56:C6:1F:1F:9C:49:AF:0B:45:FC:FD:79:66:90:F2:07
            X509v3 Authority Key Identifier: 
                keyid:1D:11:39:21:1C:E2:0D:8B:14:34:B8:36:C4:F5:34:27:09:9D:E8:7A

Certificate is to be certified until Nov 17 19:21:33 2017 GMT (365 days)
Sign the certificate? [y/n]:y


1 out of 1 certificate requests certified, commit? [y/n]y
Write out database with 1 new entries
Data Base Updated
unable to write 'random state'
[11/17/2016 11:22] seed@ubuntu:~/SEED-Labs/PKI-Lab$
```

If OpenSSL refuses to generate certificates, it is very likely that the names in
your requests do not match with those of CA. (yes, think about you are creating
a root CA for your company, and use it to sign certificates for different
departments.) The matching rules are specified in the configuration file (at the
```[policy match]``` section). You can change the names of your requests to
comply with the policy, or you can change the policy.  The configuration file
also includes another policy (called ```policy_anything```), which is less
restrictive. You can choose that policy by changing the following line:
```"policy = policy_match"  change to "policy = policy_anything".```

## 0x3 Use PKI for Web Sites

Following the lab manual didn't work out. While adding the ```ca.crt``` to
Firefox, I run into the following error that the format isn't supported by Firefox.

```
Failed to decode the file. Either it is not in PKCS #12 format, has been corrupted,
or the password you entered was incorrect.
```

Find this [stackoverflow post][1]. We should use the following command to combine the ```server.crt``` with the ```ca.crt```.

```text
openssl pkcs12 -export -in server.crt -inkey server.key -out PKILabServerFirefox.pfx -certfile ca.crt
```

Finally, we can import ```PKILabServerFirefox.pfx``` to Firefox and right
click the certificate and select "Edit Trust->this certificate can identify
websites"

## 0x4 Test the certificate

start a openssl simple webserver:

```text
seed@ubuntu:~/SEED-Labs/PKI-Lab$ cp server.key server.pem
seed@ubuntu:~/SEED-Labs/PKI-Lab$ cat server.crt >> server.pem
seed@ubuntu:~/SEED-Labs/PKI-Lab$ openssl s_server -cert server.pem -www -accpet 6666
```

Now navigate to ```https://www.pkilabserver.com:6666```, the security verification
is passed and show the openssl web server default page.

```text
s_server -cert server.pem -www -accept 3633 
Ciphers supported in s_server binary
TLSv1/SSLv3:ECDHE-RSA-AES256-GCM-SHA384TLSv1/SSLv3:ECDHE-ECDSA-AES256-GCM-SHA384
TLSv1/SSLv3:ECDHE-RSA-AES256-SHA384  TLSv1/SSLv3:ECDHE-ECDSA-AES256-SHA384
TLSv1/SSLv3:ECDHE-RSA-AES256-SHA     TLSv1/SSLv3:ECDHE-ECDSA-AES256-SHA   
TLSv1/SSLv3:SRP-DSS-AES-256-CBC-SHA  TLSv1/SSLv3:SRP-RSA-AES-256-CBC-SHA  
TLSv1/SSLv3:SRP-AES-256-CBC-SHA      TLSv1/SSLv3:DHE-DSS-AES256-GCM-SHA384
TLSv1/SSLv3:DHE-RSA-AES256-GCM-SHA384TLSv1/SSLv3:DHE-RSA-AES256-SHA256    
TLSv1/SSLv3:DHE-DSS-AES256-SHA256    TLSv1/SSLv3:DHE-RSA-AES256-SHA       
TLSv1/SSLv3:DHE-DSS-AES256-SHA       TLSv1/SSLv3:DHE-RSA-CAMELLIA256-SHA  
TLSv1/SSLv3:DHE-DSS-CAMELLIA256-SHA  TLSv1/SSLv3:ECDH-RSA-AES256-GCM-SHA384
TLSv1/SSLv3:ECDH-ECDSA-AES256-GCM-SHA384TLSv1/SSLv3:ECDH-RSA-AES256-SHA384   
TLSv1/SSLv3:ECDH-ECDSA-AES256-SHA384 TLSv1/SSLv3:ECDH-RSA-AES256-SHA      
TLSv1/SSLv3:ECDH-ECDSA-AES256-SHA    TLSv1/SSLv3:AES256-GCM-SHA384        
TLSv1/SSLv3:AES256-SHA256            TLSv1/SSLv3:AES256-SHA               
TLSv1/SSLv3:CAMELLIA256-SHA          TLSv1/SSLv3:PSK-AES256-CBC-SHA       
TLSv1/SSLv3:ECDHE-RSA-DES-CBC3-SHA   TLSv1/SSLv3:ECDHE-ECDSA-DES-CBC3-SHA 
TLSv1/SSLv3:SRP-DSS-3DES-EDE-CBC-SHA TLSv1/SSLv3:SRP-RSA-3DES-EDE-CBC-SHA 
TLSv1/SSLv3:SRP-3DES-EDE-CBC-SHA     TLSv1/SSLv3:EDH-RSA-DES-CBC3-SHA     
TLSv1/SSLv3:EDH-DSS-DES-CBC3-SHA     TLSv1/SSLv3:ECDH-RSA-DES-CBC3-SHA    
TLSv1/SSLv3:ECDH-ECDSA-DES-CBC3-SHA  TLSv1/SSLv3:DES-CBC3-SHA             
TLSv1/SSLv3:PSK-3DES-EDE-CBC-SHA     TLSv1/SSLv3:ECDHE-RSA-AES128-GCM-SHA256
TLSv1/SSLv3:ECDHE-ECDSA-AES128-GCM-SHA256TLSv1/SSLv3:ECDHE-RSA-AES128-SHA256  
TLSv1/SSLv3:ECDHE-ECDSA-AES128-SHA256TLSv1/SSLv3:ECDHE-RSA-AES128-SHA     
TLSv1/SSLv3:ECDHE-ECDSA-AES128-SHA   TLSv1/SSLv3:SRP-DSS-AES-128-CBC-SHA  
TLSv1/SSLv3:SRP-RSA-AES-128-CBC-SHA  TLSv1/SSLv3:SRP-AES-128-CBC-SHA      
TLSv1/SSLv3:DHE-DSS-AES128-GCM-SHA256TLSv1/SSLv3:DHE-RSA-AES128-GCM-SHA256
TLSv1/SSLv3:DHE-RSA-AES128-SHA256    TLSv1/SSLv3:DHE-DSS-AES128-SHA256    
TLSv1/SSLv3:DHE-RSA-AES128-SHA       TLSv1/SSLv3:DHE-DSS-AES128-SHA       
TLSv1/SSLv3:DHE-RSA-SEED-SHA         TLSv1/SSLv3:DHE-DSS-SEED-SHA         
TLSv1/SSLv3:DHE-RSA-CAMELLIA128-SHA  TLSv1/SSLv3:DHE-DSS-CAMELLIA128-SHA  
TLSv1/SSLv3:ECDH-RSA-AES128-GCM-SHA256TLSv1/SSLv3:ECDH-ECDSA-AES128-GCM-SHA256
TLSv1/SSLv3:ECDH-RSA-AES128-SHA256   TLSv1/SSLv3:ECDH-ECDSA-AES128-SHA256 
TLSv1/SSLv3:ECDH-RSA-AES128-SHA      TLSv1/SSLv3:ECDH-ECDSA-AES128-SHA    
TLSv1/SSLv3:AES128-GCM-SHA256        TLSv1/SSLv3:AES128-SHA256            
TLSv1/SSLv3:AES128-SHA               TLSv1/SSLv3:SEED-SHA                 
TLSv1/SSLv3:CAMELLIA128-SHA          TLSv1/SSLv3:PSK-AES128-CBC-SHA       
TLSv1/SSLv3:ECDHE-RSA-RC4-SHA        TLSv1/SSLv3:ECDHE-ECDSA-RC4-SHA      
TLSv1/SSLv3:ECDH-RSA-RC4-SHA         TLSv1/SSLv3:ECDH-ECDSA-RC4-SHA       
TLSv1/SSLv3:RC4-SHA                  TLSv1/SSLv3:RC4-MD5                  
TLSv1/SSLv3:PSK-RC4-SHA              TLSv1/SSLv3:EDH-RSA-DES-CBC-SHA      
TLSv1/SSLv3:EDH-DSS-DES-CBC-SHA      TLSv1/SSLv3:DES-CBC-SHA              
---
Reused, TLSv1/SSLv3, Cipher is ECDHE-RSA-AES256-SHA
SSL-Session:
    Protocol  : TLSv1
    Cipher    : ECDHE-RSA-AES256-SHA
    Session-ID: 124BFE9E98B4DF8E5DC0CC87DEB16DDE615FEC0B63A5FD687CD7033012639DD1
    Session-ID-ctx: 01000000
    Master-Key: A23B112D5A56B98709D5C140EEC5876155C6023CF8D5DCD81858B1EFF3B4E9CE8723E2D9703C217D8C517A762820B32C
    Key-Arg   : None
    PSK identity: None
    PSK identity hint: None
    SRP username: None
    Start Time: 1479414975
    Timeout   : 300 (sec)
    Verify return code: 0 (ok)
---
   0 items in the session cache
   0 client connects (SSL_connect())
   0 client renegotiates (SSL_connect())
   0 client connects that finished
  16 server accepts (SSL_accept())
   0 server renegotiates (SSL_accept())
  16 server accepts that finished
   3 session cache hits
   0 session cache misses
   0 session cache timeouts
   0 callback cache hits
   0 cache full overflows (128 allowed)
---
no client certificate available
```

## 0x5 RSA and AES encryption performace

We use the following commands to evaluate RSA encryption and decryption:

```bash
# generate private key
openssl genrsa -out private-rsa.pem 1024
# generate public key
openssl genrsa -in private-rsa.pem -pubout -out public-rsa.pem
# encrypt using public key
openssl rsautl -encrypt -pubin -inkey public-rsa.pem -in message.txt -out message_enc.txt
# decrypt using private key
openssl rsautl -decrypt -inkey public-rsa.pem -in message_enc.txt -out message_dec.txt
```

Commands for AES encrytption and decryption

```
TODO
```

[1]: http://unix.stackexchange.com/questions/73873/firefox-error-message-when-adding-client-certificate-signed-by-ca) "Stackoverflow Post"