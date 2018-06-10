input="yes"
while [ $input = "yes" ]
do

    select math in Addition Subtraction Multiplication Division
    do
        case "$math" in
        Addition)
            #Write your code here
        ;;
        Subtraction)
            #Write your code here
        ;;
        Multiplication)
            #Write your code here
        ;;
        Division)
            #Write your code here
        ;;
        *)
            echo Choose 1 to 4 only!!!!
            break
        ;;
    esac
    done

done
