import type { Express } from "express";
import type { Server } from "http";
import { storage } from "./storage";
import { api } from "@shared/routes";
import { z } from "zod";

export async function registerRoutes(
  httpServer: Server,
  app: Express
): Promise<Server> {
  app.get(api.messages.list.path, async (req, res) => {
    const messages = await storage.getMessages();
    res.json(messages);
  });

  app.post(api.messages.create.path, async (req, res) => {
    const input = api.messages.create.input.parse(req.body);
    const message = await storage.createMessage(input);
    res.status(201).json(message);
  });

  // Seed data
  const existingMessages = await storage.getMessages();
  if (existingMessages.length === 0) {
    await storage.createMessage({ content: "System initialized" });
    await storage.createMessage({ content: "Ready for package analysis" });
  }

  return httpServer;
}
