# declare -a videos=("center3" "center4" "up3" "up4" "down3" "down4" "left3" "left4" "right3_1" "right3_2" "right_4" "tv_right_3" "tv_right_4")
# declare -a videos=("center1" "center2" "down1" "down2" "left1" "left2" "right1" "right2" "tv1" "tv2" "tv3" "up1" "up2")
## now loop through the above array
#mkdir $2
FILES="${1}/*mp4"
center=1
down=1
left=1
right=1
up=1
tv=1
for i in $FILES
do
    date
    echo "$i"
    if [[ $i == *center* ]]; then
        output="center${center}.csv"
        center=$((center+1))
    fi

    if [[ $i == *up* ]]; then
        output="up${up}.csv"
        up=$((up+1))
    fi

    if [[ $i == *down* ]]; then
        output="down${down}.csv"
        down=$((down+1))
    fi

    if [[ $i == *left* ]]; then
        output="left${left}.csv"
        left=$((left+1))
    fi

    if [[ $i == *right* ]]; then
        output="right${right}.csv"
        right=$((right+1))
    fi

    if [[ $i == *tv* ]]; then
        output="tv${tv}.csv"
        tv=$((tv+1))
    fi
    D:/RAP_Openpose/x64/Release/RAP_Openpose.exe -video $i -write_face_keypoints_file "${2}/${output}"
    echo
done
