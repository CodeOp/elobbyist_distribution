<?php

//using this interface and classes it will be easy to replace/add more mail sending methods

class Elobbyist_MailInfo {
  public $From = NULL;
  public $To = NULL;
  public $Subject = NULL;
  public $Body = NULL;
  public $Host = NULL;
  public $Port = '';
  public $Password = NULL;
}

class Elobbyist_MailResponse {
  public $Success = FALSE;
  public $Bounced = FALSE;
  public $ErrorMessage = NULL;
  public $ErrorCode = NULL;
}

interface Elobbyist_IMail {   
  /**
   * Sends a mail
   *
   * @param $oInfo
   * A Elobbyist_MailInfo instance
   *
   * @return Elobbyist_MailResponse
   *   
   */
  public function SendMail($oInfo);
}