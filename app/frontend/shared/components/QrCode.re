[@deriving jsProperties]
type nonrec imageSettings = {
  src: string,
  [@mel.optional]
  x: option(int),
  [@mel.optional]
  y: option(int),
  [@mel.optional]
  height: option(int),
  [@mel.optional]
  width: option(int),
  [@mel.optional]
  excavate: option(bool),
};
[@mel.module "qrcode.react"] [@react.component]
external make:
  (
    ~value: string,
    ~size: int=?,
    ~bgColor: string=?,
    ~fgColor: string=?,
    ~level: string=?,
    ~includeMargin: bool=?,
    ~imageSettings: imageSettings=?,
    ~style: ReactDOM.style=?,
    ~className: string=?
  ) =>
  React.element =
  "QRCodeCanvas";
