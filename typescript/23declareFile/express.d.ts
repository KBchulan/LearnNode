// 我们可以手写声明文件
declare module 'express' {
  // 导出主函数
  export default function express(): Express;

  // Express 应用类型
  export interface Express {
    get(path: string, handler: RequestHandler): void;
    post(path: string, handler: RequestHandler): void;
    listen(port: number, callback?: () => void): void;
  }

  // 请求对象
  export interface Request {
    body: any;
    params: Record<string, string>;
    query: Record<string, string>;
    headers: Record<string, string>;
  }

  // 响应对象
  export interface Response {
    send(body: any): void;
    json(body: any): void;
    status(code: number): Response;
  }

  // 中间件处理函数
  export type RequestHandler = (
    req: Request,
    res: Response,
    next: () => void
  ) => void;
}
