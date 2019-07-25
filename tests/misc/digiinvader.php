<?php

// script for generating variations of a digiinvaders stream csp
// run with php -f digiinvader.php <length> > <outfile>


$length = $argv[1];

echo "var I : [-2, 9];\n";
for ($i = 0; $i < $length; $i++) {
    echo "var D$i : [-1, 9];\n";
}
for ($i = 0; $i < $length; $i++) {
    echo "var A$i : [0, 1];\n";
}
echo "var MISS : [0, 1];\n";
echo "var GAMEOVER : [0, 1];\n";

echo "\n";

echo "I != -1;\n";

echo "\n";

for ($i = 0; $i < $length - 1; $i++) {
    echo "first D$i == -1;\n";
}
echo "D" . ($length - 1) . " == 0 fby 1 fby 2 fby D" . ($length - 1) . ";\n";

echo "\n";

for ($i = 0; $i < $length; $i++) {
    echo "A$i ==";
    for ($j = 0; $j < $i; $j++) {
        echo " I ne D$j and";
    }
    echo " I eq D$i;\n";
}

echo "\n";

echo "MISS == (";
for ($i = 0; $i < $length; $i++) {
    if ($i != 0) {
        echo " + ";
    }
    echo "A$i";
}
echo ") eq 0;\n";
echo "GAMEOVER == D0 ne -1 and MISS;\n";

echo "\n";

for ($i = 0; $i < $length - 1; $i++) {
    echo "next D$i == if GAMEOVER then -1 else if MISS";
    for ($j = 0; $j <= $i; $j++) {
        echo " or A$j";
    }
    echo " then D" . ($i + 1) . " else D$i;\n";
}
?>
