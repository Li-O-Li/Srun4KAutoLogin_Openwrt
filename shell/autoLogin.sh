#!/bin/ash
name="buaaSrunCertificate"

# advanced settings
# typically, you do not need to modify the following settings
# if you want to modify, please make sure you know what you are doing
action=login
ac_id=1
n=200
type=1
os='Windows 10'
name=Windows
double_stack=0

# init Settings(please modify)
username=testuser                                       # user name
passwordPlain=testpwd                                   # plain password
ipSchoolGateway="10.200.21.4"                          # BUAA gateway ip
executablePath="[EXECUTABLE_PATH]"                     # path to srunRequestBodyGeneration
challengeURL="https://${ipSchoolGateway}/cgi-bin/get_challenge" # Certificate server URLs
portalURL="https://${ipSchoolGateway}/cgi-bin/srun_portal"

user_agent="Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/105.0.0.0 Safari/537.36 Edg/105.0.1343.33"
callback=""
challenge=""
logPath="[LOG_PATH]"                                    # log path, can be fulfilled by install.sh

# STEP1: get challenge
getChallenge(){
    # step 1: perparing post data
    # field0:URL
    local URL=$challengeURL
    # field1:jQuery(no use for the program, just generate a random string to fill)
    local randNum21Bit=""
    local bit1_4="1124"
    local bit5_8=$(printf "%04d" $((RANDOM % 10000)))
    local bit9_12=$(printf "%04d" $((RANDOM % 10000)))
    local bit13_16=$(printf "%04d" $((RANDOM % 10000)))
    local bit16_20=$(printf "%04d" $((RANDOM % 10000)))
    local bit21=$(printf "%01d" $((RANDOM % 10)))
    local randNum21Bit=$bit1_4$bit5_8$bit9_12$bit13_16$bit16_20$bit21
    timeMilli=$(date +%s%3N)
    callback="jQuery"$randNum21Bit"_"$timeMilli
    # field2:get ipv4 addr
    ip=$(ubus call network.interface.wan status | grep \"address\" | grep -oE '[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}')
    
    # step 2:generate post string
    local post_data="callback="$callback"&username="$username"&ip="$ip"&_="$timeMilli

    # step 3:send GET request to certificate server and parse response
    echo "requestURL=${URL}?${post_data}" >> $logPath
    local server_response=$(curl --insecure --max-time 3 -s --user-agent "$user_agent" "${URL}?${post_data}")
    echo "response=${server_response}"  >>$logPath
    local res=$(echo "$server_response" | grep -o '"res": *"[^"]*"' | sed 's/"res":"\([^"]*\)"/\1/')
    # echo $res

    # step 4:check result
    if [ "$res" == "ok" ]; then
        challenge=$(echo "$server_response" | grep -o '"challenge": *"[^"]*"' | sed 's/"challenge":"\([^"]*\)"/\1/')
        # echo "challenge fetched!"$challenge >>$logPath
        return 0    
    else
        echo "challenge fetch FAILED.challenge="$challenge >>$logPath
        echo "corresponding header="$post_data >>$logPath
        echo "response="$server_response >>$logPath
        challenge=""
        return 1
    fi
}


# STEP2: portal login
portalLogin(){

    local URL=$portalURL
    getChallenge
    if [ $challenge != "" ]; then
	    echo "valid token. "$challenge >>$logPath
    else
	    echo "invalid token" >>$logPath
	    return 2
    fi

    timeMilli=$(date +%s%3N)
    timeOutput=$(date "+%Y-%m-%d %H:%M:%S")
    # generate request body using extern C-language-based executable
    local requestBody=$($executablePath $callback $username $passwordPlain $ip $timeMilli $challenge $action $ac_id $n $type $os $name $double_stack)
    # send login request to server
    local server_response=$(curl --insecure --max-time 3 -s --user-agent "$user_agent" "${URL}?${requestBody}")
    # analyze result
    local res=$(echo "$server_response" | grep -o '"suc_msg": *"[^"]*"' | sed 's/"suc_msg":"\([^"]*\)"/\1/')
    if [ "$res" == "login_ok" ]; then
        echo "successfully login at "$timeOutput >>$logPath
        return 0
    else
        echo "login failed at "$timeOutput >>$logPath
        echo "result shows "$res >>$logPath
        echo "requestBody="$requestBody >>$logPath
        return 3
    fi
}

# loop check if connectivity is good
checkConnectivity(){
    while :;
    do
    timeOutput=$(date "+%Y-%m-%d %H:%M:%S")
    # ipv6 connection DO NOT need certificate
    # ping baidu in only-ipv4 mode to check if connectivity is good
    local checker=$(curl --max-time 3 -4 -s -I 220.181.38.149 --user-agent "$user_agent" 2>&1 | grep "HTTP/" | awk '{print $2}')
    echo "check for connectivity shows:"$checker >> $logPath
    if [ "$checker" == "302" ]; then  # status code 302 - 'found', connectivity is good
        echo "online at"$timeOutput >> $logPath
        if [ -e "/tmp/heartBeatPackage.tmp" ]; then
            $(rm -f "/tmp/heartBeatPackage.tmp")
        fi
        sleep 5
        local tmp=$(curl -s 220.181.38.149 -o "/tmp/heartBeatPackage.tmp" --user-agent "$user_agent")
        sleep 295                     # if online, only check every 5 min
    else
        echo "User is offline, trying to reconnect at "$timeOutput >> $logPath
        portalLogin
    fi
    # 5~30s retry interval between retries
    sleep 5
    sleep $((RANDOM % 25))
    done
}

checkConnectivity