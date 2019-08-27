<?php
$script = file_get_contents("./test.ekwa");
$lines = explode("\n", $script);
$bytecode = NULL;

$tokens = array(
	"EKWA_VAR"	=> "\x01", 
	"EKWA_BUFF"	=> "\x02",
	"EKWA_ARG"	=> "\x03",
	"EKWA_CALL"	=> "\x04",
	"EKWA_JMP"	=> "\x05",
	"EKWA_FSET"	=> "\x06",
	"EKWA_WRT"	=> "\x07",
	"EKWA_IFE"	=> "\x08",
	"EKWA_IFNE"	=> "\x09",
	"EKWA_IFS"	=> "\x0a",
	"EKWA_IFB"	=> "\x0b",
	"EKWA_INFO"	=> "\x0c",
	"EKWA_SHOW"	=> "\x0d",
	"EKWA_RMV"	=> "\x0e",
	"EKWA_VAL"	=> "\x0f",
	"EKWA_CAT"	=> "\x10",
	"EKWA_OPT"	=> "\x11",
	"EKWA_EXIT"	=> "\x12",
	"EKWA_PBUF"	=> "\x13",

	"EKWA_ADD"	=> "\x14",
	"EKWA_SUB"	=> "\x15",
	"EKWA_DIV"	=> "\x16",
	"EKWA_MOD"	=> "\x17",
	"EKWA_MUL"	=> "\x18",
	"EKWA_SAL"	=> "\x19",
	"EKWA_SAR"	=> "\x20",

	"EKWA_END"	=> "\x21"
);

$types = array(
	"EKWA_BYTES"	=> "\x00",
	"EKWA_FLOAT"	=> "\x01",
	"EKWA_INT"		=> "\x02",
	"EKWA_PTR"		=> "\x03"
);

$arr = array();
$vars = array();
$i = 0;



foreach ($lines as $line) {
	$elements = explode("\t", $line);

	if (!isset($tokens[$elements[0]])) {
		continue;
	}

	//$bytecode .= $tokens[$elements[0]];
	$arr[$i]['token'] = $tokens[$elements[0]];

	if (!isset($elements[1])) {
		$arr[$i]['len1'] = "\x00\x00";
		$i++;
		//$bytecode .= "\x00\x00";
		continue;
	}

	$length = strlen($elements[1]);
	$arr[$i]['len1'] = pack('n', $length);
	$arr[$i]['arg1'] = $elements[1];

	if (!isset($elements[2])) {
		$arr[$i]['len2'] = "\x00\x00";
		$i++;
		//$bytecode .= "\x00\x00";
		continue;
	}

	if (isset($types[$elements[2]])
		&& $elements[0] == "EKWA_VAR") {
		$arr[$i]['len2'] = "\x00\x01";
		$arr[$i]['arg2'] = $types[$elements[2]];
		$vars[$elements[1]] = $elements[2];
		$i++;
		continue;
	}
/*
	if ($elements[0] == "EKWA_VAL") {
		if ($vars[$elements[1]] == "EKWA_INT") {
			$d_bytes = pack('i', (int)$elements[2]);
			$length = strlen($d_bytes);
			$bytecode .= pack('n', $length);
			$bytecode .= $d_bytes;
			continue;
		}
		if ($vars[$elements[1]] == "EKWA_FLOAT") {
			$d_bytes = pack('f', (float)$elements[2]);
			$length = strlen($d_bytes);
			$bytecode .= pack('n', $length);
			$bytecode .= $d_bytes;
			continue;
		}
	}
*/
	$length = strlen($elements[2]);
	$arr[$i]['len2'] = pack('n', $length);
	$arr[$i]['arg2'] = (string)$elements[2];
	$i++;
}

foreach ($arr as $one) {
	echo "token: " . bin2hex($one['token']) . "\n";
	echo "len1: " . bin2hex($one['len1']) . "\n";

	if (!isset($one['arg1']) || strlen($one['arg1']) == 0) {
		echo "\n\n";
		continue;
	}

	echo "arg1: " . $one['arg1'] . "\n";
	echo "len2: " . bin2hex($one['len2']) . "\n";

	if (!isset($one['arg2']) || strlen($one['arg2']) == 0) {
		echo "\n\n";
		continue;
	}

	echo "arg2: " . $one['arg2'] . "\n\n\n";
}

$bcode = $tokens['EKWA_END'];

for ($i = count($arr); $i != 0; $i--) {
	if (isset($arr[$i - 1]['arg2']) && strlen($arr[$i - 1]['arg2']) > 0) {
		$bcode .= $arr[$i - 1]['arg2'] . $arr[$i - 1]['len2'];
	}
	else {
		$bcode .= $arr[$i - 1]['len2'];
	}

	if (isset($arr[$i - 1]['arg1']) && strlen($arr[$i - 1]['arg1']) > 0) {
		$bcode .= $arr[$i - 1]['arg1'] . $arr[$i - 1]['len1'];
	}
	else {
		$bcode .= $arr[$i - 1]['len1'];
	}

	$bcode .= $arr[$i - 1]['token'];
}

file_put_contents("./vm", $bcode, FILE_APPEND);
echo "Instructions: " . bin2hex($bcode) . "\n";

   //74 6d 70 5f 62 75 66 66 00 08 0d 00 00 0c
//18 74 6d 70 5f 62 75 66 66 30 0d 00 00 0c
//18 74 6d 70 5f 62 75 66 66 00 00 0d 00 00 0c