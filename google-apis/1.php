<?php

$params = array(
                    'client_id'     =>   'FILL',
                    'redirect_uri'  =>   'FILL',
                    'scope'         =>   'FILL',
                    'response_type' =>   'code'
               );

$url = 'https://accounts.google.com/o/oauth2/auth?' . http_build_query($params);        
echo $url."\n";
?>
