<?php

require_once "Mail.php";
require_once dirname(__FILE__) . '/Elobbyist_IMail.inc';

class Elobbyist_Mail implements Elobbyist_IMail {
  /*
   * Implements mail sending by PEAR Mail
  */
  public function SendMail($oInfo) {
    
    $oResponse = new Elobbyist_MailResponse();
    
    // Build the default headers
    $message = array();
    $message['headers'] = array(
      'MIME-Version'              => '1.0',
      'Content-Type'              => 'text/html; charset=UTF-8; format=flowed; delsp=yes',
      'Content-Transfer-Encoding' => '8Bit',
    );
    //subject is in the headers in this mail function, so copy it
    $message['headers']['Subject'] = $oInfo->Subject;
    $message['headers']['From'] = $message['headers']['Sender'] = $message['headers']['Return-Path'] = $oInfo->From;
    $message['headers']['To'] = $oInfo->To;
    $message['body'] = $oInfo->Body;
            
    $smtp = @Mail::factory('smtp',
        array ('host' => $oInfo->Host,
          'port' => $oInfo->Port,
          'auth' => true,
          'username' => $oInfo->From,
          'password' => $oInfo->Password));

      $result = @$smtp->send($oInfo->To, $message['headers'], $message['body']);

      if (@PEAR::isError($result)) {
        $oResponse->Success = FALSE;  
        $oResponse->ErrorCode = $result->getCode();
        switch($oResponse->ErrorCode) {
          case PEAR_MAIL_SMTP_ERROR_AUTH:
          case PEAR_MAIL_SMTP_ERROR_SENDER:
          case PEAR_MAIL_SMTP_ERROR_CONNECT:
            $oResponse->Bounced = TRUE;
            break;
          default:
            $oResponse->Bounced = FALSE;
            break;
        }
        $oResponse->ErrorMessage = $result->getMessage();
      }
      else if ($result) {
        $oResponse->Success = TRUE;
        $oResponse->Bounced = FALSE;
      }
    
    return $oResponse;
  }
}
